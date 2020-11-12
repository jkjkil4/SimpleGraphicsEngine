#pragma once

class Device
{
public:
	virtual ~Device() = default;

	virtual void begin() {}
	virtual void end() {}
};