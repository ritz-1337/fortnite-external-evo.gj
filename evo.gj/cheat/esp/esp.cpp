#include "esp.h"

void Esp::ActorLoop() {
	//I will link dumps.host to each offset, on the website it might be outdated but it gives you a understanding of the sdk
	
	//get address
	{
		LocalPtrs::Gworld = read<uint64_t>(BaseId + 0xE075F20); //https://fn.dumps.host/offsets?offset=GWorld
		if (Debug::PrintPointers) Util::PrintPtr("Uworld: ", LocalPtrs::Gworld);
		if (!LocalPtrs::Gworld) return;

		uintptr_t GameInstance = read<uint64_t>(LocalPtrs::Gworld + 0x1A8); //https://fn.dumps.host/?class=UWorld&member=OwningGameInstance
		if (Debug::PrintPointers) Util::PrintPtr("GameInstance: ", GameInstance);
		if (!GameInstance) return;

		LocalPtrs::LocalPlayers = read<uint64_t>(read<uint64_t>(GameInstance + 0x38)); //https://fn.dumps.host/?class=UGameInstance&member=LocalPlayers
		if (Debug::PrintPointers) Util::PrintPtr("LocalPlayers: ", LocalPtrs::LocalPlayers);
		if (!LocalPtrs::LocalPlayers) return;

		LocalPtrs::PlayerController = read<uint64_t>(LocalPtrs::LocalPlayers + 0x30); //https://fn.dumps.host/?class=UPlayer&member=PlayerController
		if (Debug::PrintPointers) Util::PrintPtr("PlayerController: ", LocalPtrs::PlayerController);
		if (!LocalPtrs::PlayerController) return;
		
		LocalPtrs::Player = read<uint64_t>(LocalPtrs::PlayerController + 0x328); //https://fn.dumps.host/?class=APlayerController&member=AcknowledgedPawn
		if (Debug::PrintPointers) Util::PrintPtr("Player: ", LocalPtrs::Player);
		if (!LocalPtrs::Player) return;

		LocalPtrs::RootComponent = read<uint64_t>(LocalPtrs::Player + 0x190); //https://fn.dumps.host/?class=AActor&member=RootComponent
		if (Debug::PrintPointers) Util::PrintPtr("RootComponent: ", LocalPtrs::RootComponent);
		if (!LocalPtrs::RootComponent) return;

	}

	//get player array then loop through it
	{
		uintptr_t GameState = read<uintptr_t>(LocalPtrs::Gworld + 0x148); //https://fn.dumps.host/?class=UWorld&member=GameState
		if (Debug::PrintPointers) Util::PrintPtr("GameState: ", GameState);
		if (!GameState) return;

		uintptr_t PlayerArrayOffset = 0x2a0; //https://fn.dumps.host/?class=AGameStateBase&member=PlayerArray
		//PlayerArray is stored as an array in the sdk, it consist of a list of all the player states in the current game
		uintptr_t PlayerArray = read<uintptr_t>(GameState + PlayerArrayOffset); 

		int Num = read<int>(GameState + (PlayerArrayOffset + sizeof(uintptr_t))); //reads the total number of player states in this array
		
		for (int i = 0; i < Num; i++) {
		

			uintptr_t PlayerState = read<uintptr_t>(PlayerArray + (i * sizeof(uintptr_t))); //the size of the pointer in the array we are reading is the size of uintptr_t
			if (Debug::PrintPointers) Util::PrintPtr("Enemy PlayerState: ", PlayerState);
			if (!PlayerState) continue; // we say if the pointer is invalid to continue, this means it will continue onto the next loop

			//since we only have the player state, we use PawnPrivate to get to the player
			uintptr_t Player = read<uintptr_t>(PlayerState + 0x300); //https://fn.dumps.host/?class=APlayerState&member=PawnPrivate
			if (Debug::PrintPointers) Util::PrintPtr("Enemy Player: ", Player);
			if (!Player) continue;
			if (Player == LocalPtrs::Player) continue; //if this current player we are looping through equals our local player then continue, so we dont draw esp on ourselves

			uintptr_t Mesh = read<uintptr_t>(Player + 0x310); //https://fn.dumps.host/?class=ACharacter&member=Mesh
			if (Debug::PrintPointers) Util::PrintPtr("Enemy Mesh: ", Mesh);
			if (!Mesh) continue;

			Vector3 Head3D = SDK::GetBoneWithRotation(Mesh, 68);
			Vector2 Head2D = SDK::ProjectWorldToScreen(Head3D);
			if (Debug::PrintLocations) Util::Print3D("Head3D: ", Head3D);
			if (Debug::PrintLocations) Util::Print2D("Head2D: ", Head2D);

			Vector3 Bottom3D = SDK::GetBoneWithRotation(Mesh, 0);
			Vector2 Bottom2D = SDK::ProjectWorldToScreen(Bottom3D);

			float BoxHeight = (float)(Head2D.y - Bottom2D.y);
			float CornerHeight = abs(Head2D.y - Bottom2D.y);
			float CornerWidth = BoxHeight * 0.80;

			if (bCornerBox)
				Util::DrawCornerBox(Head2D.x - (CornerWidth / 2), Head2D.y, CornerWidth, CornerHeight, IM_COL32(0, 173, 237, 255), 1.5);
			
			auto dist = Util::GetCrossDistance(Head2D.x, Head2D.y, Width / 2, Height / 2);
			if (dist < FovSize && dist < ClosestDistance) {
				ClosestDistance = dist;
				TargetPawn = Player;
			}
		}
	}
}
