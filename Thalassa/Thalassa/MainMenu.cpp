#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "utils.hpp"
#include "MainMenu.hpp"
#include "AEEngine.h"
#include <iostream>

//// Menu selection state
//MenuOption selectedOption = MENU_PLAY;
//
//AEGfxVertexList* buttonMesh = nullptr; // button meshes
//
//int Thalassa_Counter = 0;
//
//void MainMenu_Load()
//{
//	std::cout << "Level1:Load" << std::endl;
//}
//
//void MainMenu_Init()
//{
//	std::cout << "Level1:Initialize" << std::endl;
//	selectedOption = MENU_PLAY; // Default selection
//
//}
//
////void MainMenu_Update()
////{
////	std::cout << "Level1:Update" << std::endl;
////	if (AEInputCheckTriggered(AEVK_DOWN)) {
////		selectedOption = static_cast<MenuOption>((selectedOption + 1) % MENU_TOTAL); 
////	}
////	if (AEInputCheckTriggered(AEVK_UP)) {
////		selectedOption = static_cast<MenuOption>((selectedOption - 1 + MENU_TOTAL) % MENU_TOTAL);
////	}
////
////	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
////		switch (selectedOption) {
////			case MENU_PLAY:
////				GameStateManager::SetState(GS_THALASSA);
////				break;
////			case MENU_CREDITS:
////				GameStateManager::SetState(GS_CREDITS);
////				break;
////			case MENU_EXIT:
////				AESysExit();
////				break;
////		}
////	}
////}
//
//void MainMenu_Draw()
//{
//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
//	AEGfxSetBackgroundColor(0, 0, 0); 
//
//	float buttonWidth = 200.0f;
//	float buttonHeight = 50.0f;
//	float startY = 100.0f;
//	float spacing = 80.0f;
//	
//	AEMtx33 playTransform = createTransformMtx(buttonWidth, buttonHeight, 0, 0, startY);
//
//
//}
//
//void MainMenu_Free()
//{
//	std::cout << "Level1:Free" << std::endl;
//}
//
//void MainMenu_Unload()
//{
//	std::cout << "Level1:Unload" << std::endl;
//}