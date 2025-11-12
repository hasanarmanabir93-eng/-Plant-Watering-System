#ifndef CAPACITIVE_SOIL_MOISTURE_SENSOR_H
#define CAPACITIVE_SOIL_MOISTURE_SENSOR_H

#include <SinricProDevice.h>
#include <Capabilities/ModeController.h>
#include <Capabilities/RangeController.h>
#include <Capabilities/PushNotification.h>

class CapacitiveSoilMoistureSensor
  : public SinricProDevice,
    public ModeController<CapacitiveSoilMoistureSensor>,
    public RangeController<CapacitiveSoilMoistureSensor>,
    public PushNotification<CapacitiveSoilMoistureSensor> {
  
  friend class ModeController<CapacitiveSoilMoistureSensor>;
  friend class RangeController<CapacitiveSoilMoistureSensor>;
  friend class PushNotification<CapacitiveSoilMoistureSensor>;

public:
  explicit CapacitiveSoilMoistureSensor(const String &deviceId)
    : SinricProDevice(deviceId, "CapacitiveSoilMoistureSensor") {}
};

#endif  // CAPACITIVE_SOIL_MOISTURE_SENSOR_H
