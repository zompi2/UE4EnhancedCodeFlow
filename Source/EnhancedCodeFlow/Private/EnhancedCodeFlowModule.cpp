// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

#include "Modules/ModuleManager.h"
#include "ECFLogs.h"

DEFINE_LOG_CATEGORY(LogECF);

class ENHANCEDCODEFLOW_API FEnhancedCodeFlowModule : public IModuleInterface
{};

IMPLEMENT_MODULE(FEnhancedCodeFlowModule, EnhancedCodeFlow)
