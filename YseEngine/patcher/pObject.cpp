#include "pObject.h"
#include "pHandle.hpp"
#include "headers\enums.hpp"
#include "implementations\logImplementation.h"

using namespace YSE::PATCHER;

unsigned int LastPatcherObjectID = 0;
unsigned int pObject::CreateID() {
  return LastPatcherObjectID++;
}

pObject::pObject(bool isDSPObject) 
  : DSP(isDSPObject)
  , ID(CreateID())
  , handler(nullptr)
  , guiInt(nullptr)
  , guiFlt(nullptr)
{}

bool pObject::IsDSPStartPoint() {
  if(!DSP) return false;

  for (unsigned int i = 0; i < inputs.size(); i++) {
    if (inputs[i].HasActiveDSPConnection()) return false;
  }

  // at this point either the dsp pin is not connected or there is none
  return true;
}

void pObject::ResetDSP() {
  for (unsigned int i = 0; i < inputs.size(); i++) {
    inputs[i].ResetDSP();
  }
}

void pObject::CalculateIfReady() {
  // make sure all dsp inputs are ready
  for (unsigned int i = 0; i < inputs.size(); i++) {
    if (inputs[i].WaitingForDSP()) {
      return;
    }
  }
  Calculate();
  UpdateGui();
}

void pObject::SetParams(const std::string & args) {
  parms.Set(args);
}

void pObject::ConnectInlet(outlet * from, int inlet)
{
  inputs[inlet].Connect(from);
}

void pObject::DisconnectInlet(outlet * from, int inlet) {
  inputs[inlet].Disconnect(from);
}

void pObject::ConnectOutlet(inlet * dest, int outlet) {
  outputs[outlet].Connect(dest);
}

YSE::OUT_TYPE pObject::GetOutputType(unsigned int output) const {
  if (output < 0 || output >= outputs.size()) return OUT_TYPE::INVALID;

  return outputs[output].Type();
}


YSE::PATCHER::inlet * pObject::GetInlet(int number) {
  return &(inputs[number]);
}

YSE::PATCHER::outlet * pObject::GetOutlet(int number) {
  return &(outputs[number]);
}

void pObject::UpdateGui() {
  if (handler == nullptr) return;
  if (guiInt != nullptr) {
    handler->ToGui(ID, *guiInt);
  }
  else if (guiFlt != nullptr) {
    handler->ToGui(ID, *guiFlt);
  }
}

void pObject::RegisterGuiHandler(YSE::guiHandler * handler) {
  this->handler = handler;
}

void pObject::DumpJson(nlohmann::json::value_type & json) {
  json["type"] = Type();
  json["ID"] = ID;
  json["posX"] = pos.x;
  json["posY"] = pos.y;
  json["parms"] = parms.Get();

  for (int i = 0; i < outputs.size(); i++) {
    outputs[i].DumpJSON(json["outputs"]["output " + std::to_string(i)]);
  }
}

const std::string & pObject::GetParams() {
  return parms.Get();
}

unsigned int pObject::GetConnections(unsigned int outlet) {
  return outputs[outlet].GetConnections();
}

unsigned int pObject::GetConnectionTarget(unsigned int outlet, unsigned int connection) {
  return outputs[outlet].GetTarget(connection);
}

unsigned int pObject::GetConnectionTargetInlet(unsigned int outlet, unsigned int connection) {
  return outputs[outlet].GetTargetInlet(connection);
}