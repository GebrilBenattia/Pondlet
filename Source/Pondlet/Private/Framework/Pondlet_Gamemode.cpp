// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/Pondlet_Gamemode.h"
#include "Framework/Pondlet_Gamestate.h"


APondlet_Gamemode::APondlet_Gamemode()
{
	GameStateClass = APondlet_GameState::StaticClass();
}