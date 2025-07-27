#include "AutoResponseModule.h"
#include "MeshService.h"
#include "NodeDB.h"
#include "main.h"
#include "mesh/generated/meshtastic/cannedmessages.pb.h"
#include "mesh/generated/meshtastic/mesh.pb.h"
#include <string>

#define VERSION "1.5"

extern meshtastic_CannedMessageModuleConfig cannedMessageModuleConfig;

AutoResponseModule::AutoResponseModule() : ProtobufModule("autoresponse") {}

bool AutoResponseModule::handleReceived(const meshtastic_MeshPacket &mp)
{
    LOG_ERROR("AutoResponseModule received a message");
    if (mp.decoded.portnum != meshtastic_PortNum_TEXT_MESSAGE_APP)
    {
        LOG_ERROR("Message is not a text message, skipping");
        return false;
    }

    std::string message((char *)mp.decoded.payload.bytes, mp.decoded.payload.size);
    if (message != "hi")
    {
        LOG_ERROR("Message is not 'hi', skipping");
        return false;
    }

    if (cannedMessageModuleConfig.messages == nullptr || strlen(cannedMessageModuleConfig.messages) == 0)
    {
        LOG_ERROR("Canned messages are empty, skipping");
        return false;
    }

    if (millis() - lastResponseTime < 30000)
    {
        LOG_ERROR("Rate limit exceeded, skipping");
        return false;
    }

    meshtastic_NodeInfoLite *node = nodeDB->getMeshNode(mp.from);
    if (!node)
    {
        LOG_ERROR("Node not found, skipping");
        return false;
    }

    // Only respond to direct messages or messages on the "Testing" channel
    if (mp.to != nodeDB->getNodeNum() && strcmp(channels.getName(mp.channel), "Testing") != 0)
    {
        LOG_ERROR("Message is not a direct message or from the 'Testing' channel, skipping");
        return false;
    }

    LOG_ERROR("All checks passed, sending response");
    lastResponseTime = millis();

    float rssi = mp.rx_rssi;
    LOG_ERROR(rssi != 0 ? "RSSI: %.2f" : "No RSSI info", rssi);
    float snr = mp.rx_snr;
    LOG_ERROR(snr != 0 ? "SNR: %.2f" : "No SNR info", snr);
    int start_hops = mp.hop_start;
    LOG_ERROR(start_hops != 0 ? "Start Hops: %d" : "No start hops info", start_hops);
    int used_hops = mp.hop_limit > mp.hop_start ? 0 : mp.hop_start - mp.hop_limit;
    LOG_ERROR(used_hops != 0 ? "Used Hops: %d" : "No used hops info", used_hops);
    int limit_hops = mp.hop_limit;
    LOG_ERROR(limit_hops != 0 ? "Limit Hops: %d" : "No limit hops info", limit_hops);

    char canned_messages[sizeof(cannedMessageModuleConfig.messages)];
    strcpy(canned_messages, cannedMessageModuleConfig.messages);

    char *current_node_location = strtok(canned_messages, "|");
    LOG_ERROR(current_node_location ? "Current Node Location: %s" : "No current node location info", current_node_location);
    char *autoresponse_hops_str = strtok(NULL, "|");
    int autoresponse_hops = autoresponse_hops_str ? atoi(autoresponse_hops_str) : 0;
    LOG_ERROR(autoresponse_hops != 0 ? "Auto Response Hops: %d" : "No auto response hops info", autoresponse_hops);

    char response[256];
    snprintf(response, sizeof(response),
             "Hi %s, heard %s\n"
             "last_hop_signal: rssi %.2f, snr %.2f\n"
             "message_hops: start %d, used %d, limit %d\n"
             "autoResponse_hops: start %d\n"
             "location:https://www.openstreetmap.org/#map=16/%.5f/%.5f\n"
             "Version: %s",
             node->user.short_name, current_node_location ? current_node_location : "N/A", rssi, snr, start_hops, used_hops,
             limit_hops, autoresponse_hops,
             node->has_position ? (float)node->position.latitude_i / 10000000.0f : 0.0f,
             node->has_position ? (float)node->position.longitude_i / 10000000.0f : 0.0f, VERSION);

    LOG_ERROR("Response: %s", response);

    meshtastic_MeshPacket *p = allocDataPacket();
    p->to = mp.from;
    p->channel = mp.channel;
    p->want_ack = false;
    p->decoded.payload.size = strlen(response);
    memcpy(p->decoded.payload.bytes, response, p->decoded.payload.size);

    service->sendToMesh(p);

    LOG_ERROR("Response sent");

    return true;
}
