// Copyright AFstz.


#include "AbilitySystem/Data/CharacterClassData.h"

FCharacterClassDefaultInfo UCharacterClassData::FindCharacterClassInfo(const ECharacterClass CharacterClass)
{
	return DefaultCharacterInfo.FindChecked(CharacterClass);
}