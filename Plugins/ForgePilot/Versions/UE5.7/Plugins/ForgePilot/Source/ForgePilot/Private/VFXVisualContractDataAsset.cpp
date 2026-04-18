// Copyright 2026, HenryFX. ForgePilot Plugin.
#include "VFXVisualContractDataAsset.h"

bool UVFXVisualContractDataAsset::GetOpBinding(const FString& OpName, FVFXVisualContractOpEntry& OutBinding) const
{
	for (const FVFXVisualContractOpEntry& Entry : OpBindings)
	{
		if (Entry.OpName.Equals(OpName, ESearchCase::IgnoreCase))
		{
			OutBinding = Entry;
			return true;
		}
	}

	return false;
}

bool UVFXVisualContractDataAsset::GetParameterBinding(
	const FString& OpName,
	const FString& ParameterName,
	FVFXVisualContractBinding& OutBinding) const
{
	FVFXVisualContractOpEntry OpEntry;
	if (!GetOpBinding(OpName, OpEntry))
	{
		return false;
	}

	for (const FVFXVisualContractBinding& Binding : OpEntry.Bindings)
	{
		if (Binding.ParameterName.Equals(ParameterName, ESearchCase::IgnoreCase))
		{
			OutBinding = Binding;
			return true;
		}
	}

	return false;
}
