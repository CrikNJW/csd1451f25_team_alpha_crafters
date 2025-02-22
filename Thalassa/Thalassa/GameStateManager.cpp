#include "GameStateList.hpp"
#include "GameStateManager.hpp"
#include "utils.hpp"

//GameState GS_Current = GS_MAIN_MENU;
//GameState GS_Previous = GS_MAIN_MENU;
//GameState GS_Next = GS_MAIN_MENU;
//
//FP fpload = nullptr, fpInit = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

//void GSM_Initialize(GameState startingState) {
//	GS_Current = GS_Previous = GS_Next = startingState;
//	std::cout << "GSM:Initialize" << std::endl;
//}

//void GameStateManager::SetState(GameState newState) {
//	GS_Previous = GS_Current;  // Store the previous state
//	GS_Current = newState;     // Switch to the new state
//	GS_Next = newState;        // Ensure the next state is updated
//
//	std::cout << "Game State changed: " << GS_Current << std::endl;
//}

//void GSM_Update() {
//	std::cout << "GSM:Update" << std::endl;
//
//	switch (GS_Current) {
//	case GS_THALASSA:
//		fpLoad = Thalassa_Load;
//		fpInit = Thalassa_Init;
//		fpUpdate = Thalassa_Update;
//		fpFree = Thalassa_Free;
//		fpUnload = Thalassa_Unload;
//		break;
//	case GS_MAIN_MENU:
//		fpLoad = MainMenu_Load;
//		fpInit = MainMenu_Init;
//		fpUpdate = MainMenu_Update;
//		fpFree = MainMenu_Free;
//		fpUnload = MainMenu_Unload;
//		break;
//	case GS_LEVEL_CREATION:
//		fpLoad = LvlCreation_Load;
//		fpInit = LvlCreation_Init;
//		fpUpdate = LvlCreation_Update;
//		fpFree = LvlCreation_Free;
//		fpUnload = LvlCreation_Unload;
//		break;
//	case GS_RESTART: 
//		switch (GS_Previous) {
//		case GS_THALASSA:
//			fpLoad = Thalassa_Load;
//			fpInit = Thalassa_Init;
//			fpUpdate = Thalassa_Update;
//			fpFree = Thalassa_Free;
//			fpUnload = Thalassa_Unload;
//			break;
//		case GS_MAIN_MENU:
//			fpLoad = MainMenu_Load;
//			fpInit = MainMenu_Init;
//			fpUpdate = MainMenu_Update;
//			fpFree = MainMenu_Free;
//			fpUnload = MainMenu_Unload;
//			break;
//		default:
//			break;
//		}
//		break;
//	case GS_QUIT:
//		fpLoad = nullptr;
//		fpInit = nullptr;
//		fpUpdate = nullptr;
//		fpDraw = nullptr;
//		fpFree = nullptr;
//		fpUnload = nullptr;
//		break;
//	default:
//		fpLoad = nullptr;
//		fpInit = nullptr;
//		fpUpdate = nullptr;
//		fpDraw = nullptr;
//		fpFree = nullptr;
//		fpUnload = nullptr;
//		break;
//	}
//}