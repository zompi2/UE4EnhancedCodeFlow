// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#pragma once

#include "ECFActionBase.h"
#include "ECFStats.h"

ECF_PRAGMA_DISABLE_OPTIMIZATION

DEFINE_STAT(STAT_ECF_ActionsObjectsCount);

UECFActionBase::UECFActionBase()
{
#if STATS
	INC_DWORD_STAT(STAT_ECF_ActionsObjectsCount);
#endif
}

UECFActionBase::~UECFActionBase()
{
#if STATS
	DEC_DWORD_STAT(STAT_ECF_ActionsObjectsCount);
#endif
}

ECF_PRAGMA_ENABLE_OPTIMIZATION