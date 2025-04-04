#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	/* 
	 * Enemy Interface
	 */
	virtual void HiglightActor() override;
	virtual void UnhiglightActor() override;

protected:
	virtual void BeginPlay() override;
};
