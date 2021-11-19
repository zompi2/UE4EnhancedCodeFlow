// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

#pragma once

class FECFRandomGet
{
public:

	static constexpr int32 GetNum()
	{
		return 9990000 + __COUNTER__;
	}
};

#define ECF_INSTANCEID FECFRandomGet::GetNum()