// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItem.h"

// Add default functionality here for any IPickupItem functions that are not pure virtual.
void IPickupItem::Pickup()
{}

FModifierSet IPickupItem::GetModifiers() const
{
	return FModifierSet();
}