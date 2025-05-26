#include "AttackAnimNotifyState.h"
#include "CPPAiCharacterBase.h"


void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACPPAICharacterBase* const character = Cast<ACPPAICharacterBase>(MeshComp->GetOwner()))
		{
			character->AttackStart();
		}
	}
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp && MeshComp->GetOwner())
	{
		if (ACPPAICharacterBase* const character = Cast<ACPPAICharacterBase>(MeshComp->GetOwner()))
		{
			character->AttackEnd();
		}
	}
}
