#pragma once

#include "ProtobufModule.h"
#include "MeshTypes.h"

class AutoResponseModule : public ProtobufModule
{
  public:
    AutoResponseModule();

  protected:
    virtual bool handleReceived(const meshtastic_MeshPacket &mp) override;

  private:
    unsigned long lastResponseTime = 0;
};
