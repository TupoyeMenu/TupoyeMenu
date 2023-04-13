#pragma once
#include "entity.hpp"
#include "natives.hpp"
#include "pointers.hpp"
#include "script.hpp"
#include "vehicle.hpp"
#include "outfit.hpp"
#include "services/players/player_service.hpp"
#include "math.hpp"
#include "gta/enums.hpp"

namespace big::ped
{
	inline const std::map<int, const char*> task_names{
	    {0, "CTaskHandsUp"},
	    {1, "CTaskClimbLadder"},
	    {2, "CTaskExitVehicle"},
	    {3, "CTaskCombatRoll"},
	    {4, "CTaskAimGunOnFoot"},
	    {5, "CTaskMovePlayer"},
	    {6, "CTaskPlayerOnFoot"},
	    {8, "CTaskWeapon"},
	    {9, "CTaskPlayerWeapon"},
	    {10, "CTaskPlayerIdles"},
	    {12, "CTaskAimGun"},
	    {12, "CTaskComplex"},
	    {12, "CTaskFSMClone"},
	    {12, "CTaskMotionBase"},
	    {12, "CTaskMove"},
	    {12, "CTaskMoveBase"},
	    {12, "CTaskNMBehaviour"},
	    {12, "CTaskNavBase"},
	    {12, "CTaskScenario"},
	    {12, "CTaskSearchBase"},
	    {12, "CTaskSearchInVehicleBase"},
	    {12, "CTaskShockingEvent"},
	    {12, "CTaskTrainBase"},
	    {12, "CTaskVehicleFSM"},
	    {12, "CTaskVehicleGoTo"},
	    {12, "CTaskVehicleMissionBase"},
	    {12, "CTaskVehicleTempAction"},
	    {14, "CTaskPause"},
	    {15, "CTaskDoNothing"},
	    {16, "CTaskGetUp"},
	    {17, "CTaskGetUpAndStandStill"},
	    {18, "CTaskFallOver"},
	    {19, "CTaskFallAndGetUp"},
	    {20, "CTaskCrawl"},
	    {25, "CTaskComplexOnFire"},
	    {26, "CTaskDamageElectric"},
	    {28, "CTaskTriggerLookAt"},
	    {29, "CTaskClearLookAt"},
	    {30, "CTaskSetCharDecisionMaker"},
	    {31, "CTaskSetPedDefensiveArea"},
	    {32, "CTaskUseSequence"},
	    {34, "CTaskMoveStandStill"},
	    {35, "CTaskComplexControlMovement"},
	    {36, "CTaskMoveSequence"},
	    {38, "CTaskAmbientClips"},
	    {39, "CTaskMoveInAir"},
	    {40, "CTaskNetworkClone"},
	    {41, "CTaskUseClimbOnRoute"},
	    {42, "CTaskUseDropDownOnRoute"},
	    {43, "CTaskUseLadderOnRoute"},
	    {44, "CTaskSetBlockingOfNonTemporaryEvents"},
	    {45, "CTaskForceMotionState"},
	    {46, "CTaskSlopeScramble"},
	    {47, "CTaskGoToAndClimbLadder"},
	    {48, "CTaskClimbLadderFully"},
	    {49, "CTaskRappel"},
	    {50, "CTaskVault"},
	    {51, "CTaskDropDown"},
	    {52, "CTaskAffectSecondaryBehaviour"},
	    {53, "CTaskAmbientLookAtEvent"},
	    {54, "CTaskOpenDoor"},
	    {55, "CTaskShovePed"},
	    {56, "CTaskSwapWeapon"},
	    {57, "CTaskGeneralSweep"},
	    {58, "CTaskPolice"},
	    {59, "CTaskPoliceOrderResponse"},
	    {60, "CTaskPursueCriminal"},
	    {62, "CTaskArrestPed"},
	    {63, "CTaskArrestPed2"},
	    {64, "CTaskBusted"},
	    {65, "CTaskFirePatrol"},
	    {66, "CTaskHeliOrderResponse"},
	    {67, "CTaskHeliPassengerRappel"},
	    {68, "CTaskAmbulancePatrol"},
	    {69, "CTaskPoliceWantedResponse"},
	    {70, "CTaskSwat"},
	    {72, "CTaskSwatWantedResponse"},
	    {73, "CTaskSwatOrderResponse"},
	    {74, "CTaskSwatGoToStagingArea"},
	    {75, "CTaskSwatFollowInLine"},
	    {76, "CTaskWitness"},
	    {77, "CTaskGangPatrol"},
	    {78, "CTaskArmy"},
	    {80, "CTaskShockingEventWatch"},
	    {82, "CTaskShockingEventGoto"},
	    {83, "CTaskShockingEventHurryAway"},
	    {84, "CTaskShockingEventReactToAircraft"},
	    {85, "CTaskShockingEventReact"},
	    {86, "CTaskShockingEventBackAway"},
	    {87, "CTaskShockingPoliceInvestigate"},
	    {88, "CTaskShockingEventStopAndStare"},
	    {89, "CTaskShockingNiceCarPicture"},
	    {90, "CTaskShockingEventThreatResponse"},
	    {92, "CTaskTakeOffHelmet"},
	    {93, "CTaskCarReactToVehicleCollision"},
	    {95, "CTaskCarReactToVehicleCollisionGetOut"},
	    {97, "CTaskDyingDead"},
	    {100, "CTaskWanderingScenario"},
	    {101, "CTaskWanderingInRadiusScenario"},
	    {103, "CTaskMoveBetweenPointsScenario"},
	    {104, "CTaskChatScenario"},
	    {106, "CTaskCowerScenario"},
	    {107, "CTaskDeadBodyScenario"},
	    {114, "CTaskSayAudio"},
	    {116, "CTaskWaitForSteppingOut"},
	    {117, "CTaskCoupleScenario"},
	    {118, "CTaskUseScenario"},
	    {119, "CTaskUseVehicleScenario"},
	    {120, "CTaskUnalerted"},
	    {121, "CTaskStealVehicle"},
	    {122, "CTaskReactToPursuit"},
	    {125, "CTaskHitWall"},
	    {126, "CTaskCower"},
	    {127, "CTaskCrouch"},
	    {128, "CTaskMelee"},
	    {129, "CTaskMoveMeleeMovement"},
	    {130, "CTaskMeleeActionResult"},
	    {131, "CTaskMeleeUpperbodyAnims"},
	    {133, "CTaskMoVEScripted"},
	    {134, "CTaskScriptedAnimation"},
	    {135, "CTaskSynchronizedScene"},
	    {137, "CTaskComplexEvasiveStep"},
	    {138, "CTaskWalkRoundCarWhileWandering"},
	    {140, "CTaskComplexStuckInAir"},
	    {141, "CTaskWalkRoundEntity"},
	    {142, "CTaskMoveWalkRoundVehicle"},
	    {144, "CTaskReactToGunAimedAt"},
	    {146, "CTaskDuckAndCover"},
	    {147, "CTaskAggressiveRubberneck"},
	    {150, "CTaskInVehicleBasic"},
	    {151, "CTaskCarDriveWander"},
	    {152, "CTaskLeaveAnyCar"},
	    {153, "CTaskComplexGetOffBoat"},
	    {155, "CTaskCarSetTempAction"},
	    {156, "CTaskBringVehicleToHalt"},
	    {157, "CTaskCarDrive"},
	    {159, "CTaskPlayerDrive"},
	    {160, "CTaskEnterVehicle"},
	    {161, "CTaskEnterVehicleAlign"},
	    {162, "CTaskOpenVehicleDoorFromOutside"},
	    {163, "CTaskEnterVehicleSeat"},
	    {164, "CTaskCloseVehicleDoorFromInside"},
	    {165, "CTaskInVehicleSeatShuffle"},
	    {167, "CTaskExitVehicleSeat"},
	    {168, "CTaskCloseVehicleDoorFromOutside"},
	    {169, "CTaskControlVehicle"},
	    {170, "CTaskMotionInAutomobile"},
	    {171, "CTaskMotionOnBicycle"},
	    {172, "CTaskMotionOnBicycleController"},
	    {173, "CTaskMotionInVehicle"},
	    {174, "CTaskMotionInTurret"},
	    {175, "CTaskReactToBeingJacked"},
	    {176, "CTaskReactToBeingAskedToLeaveVehicle"},
	    {177, "CTaskTryToGrabVehicleDoor"},
	    {178, "CTaskGetOnTrain"},
	    {179, "CTaskGetOffTrain"},
	    {180, "CTaskRideTrain"},
	    {190, "CTaskMountThrowProjectile"},
	    {195, "CTaskGoToCarDoorAndStandStill"},
	    {196, "CTaskMoveGoToVehicleDoor"},
	    {197, "CTaskSetPedInVehicle"},
	    {198, "CTaskSetPedOutOfVehicle"},
	    {199, "CTaskVehicleMountedWeapon"},
	    {200, "CTaskVehicleGun"},
	    {201, "CTaskVehicleProjectile"},
	    {204, "CTaskSmashCarWindow"},
	    {205, "CTaskMoveGoToPoint"},
	    {206, "CTaskMoveAchieveHeading"},
	    {207, "CTaskMoveFaceTarget"},
	    {208, "CTaskComplexGoToPointAndStandStillTimed"},
	    {208, "CTaskMoveGoToPointAndStandStill"},
	    {209, "CTaskMoveFollowPointRoute"},
	    {210, "CTaskMoveSeekEntity_CEntitySeekPosCalculatorStandard"},
	    {211, "CTaskMoveSeekEntity_CEntitySeekPosCalculatorLastNavMeshIntersection"},
	    {212, "CTaskMoveSeekEntity_CEntitySeekPosCalculatorLastNavMeshIntersection2"},
	    {213, "CTaskMoveSeekEntity_CEntitySeekPosCalculatorXYOffsetFixed"},
	    {214, "CTaskMoveSeekEntity_CEntitySeekPosCalculatorXYOffsetFixed2"},
	    {215, "CTaskExhaustedFlee"},
	    {216, "CTaskGrowlAndFlee"},
	    {217, "CTaskScenarioFlee"},
	    {218, "CTaskSmartFlee"},
	    {219, "CTaskFlyAway"},
	    {220, "CTaskWalkAway"},
	    {221, "CTaskWander"},
	    {222, "CTaskWanderInArea"},
	    {223, "CTaskFollowLeaderInFormation"},
	    {224, "CTaskGoToPointAnyMeans"},
	    {225, "CTaskTurnToFaceEntityOrCoord"},
	    {226, "CTaskFollowLeaderAnyMeans"},
	    {228, "CTaskFlyToPoint"},
	    {229, "CTaskFlyingWander"},
	    {230, "CTaskGoToPointAiming"},
	    {231, "CTaskGoToScenario"},
	    {233, "CTaskSeekEntityAiming"},
	    {234, "CTaskSlideToCoord"},
	    {235, "CTaskSwimmingWander"},
	    {237, "CTaskMoveTrackingEntity"},
	    {238, "CTaskMoveFollowNavMesh"},
	    {239, "CTaskMoveGoToPointOnRoute"},
	    {240, "CTaskEscapeBlast"},
	    {241, "CTaskMoveWander"},
	    {242, "CTaskMoveBeInFormation"},
	    {243, "CTaskMoveCrowdAroundLocation"},
	    {244, "CTaskMoveCrossRoadAtTrafficLights"},
	    {245, "CTaskMoveWaitForTraffic"},
	    {246, "CTaskMoveGoToPointStandStillAchieveHeading"},
	    {251, "CTaskMoveGetOntoMainNavMesh"},
	    {252, "CTaskMoveSlideToCoord"},
	    {253, "CTaskMoveGoToPointRelativeToEntityAndStandStill"},
	    {254, "CTaskHelicopterStrafe"},
	    {256, "CTaskGetOutOfWater"},
	    {259, "CTaskMoveFollowEntityOffset"},
	    {261, "CTaskFollowWaypointRecording"},
	    {264, "CTaskMotionPed"},
	    {265, "CTaskMotionPedLowLod"},
	    {268, "CTaskHumanLocomotion"},
	    {269, "CTaskMotionBasicLocomotionLowLod"},
	    {270, "CTaskMotionStrafing"},
	    {271, "CTaskMotionTennis"},
	    {272, "CTaskMotionAiming"},
	    {273, "CTaskBirdLocomotion"},
	    {274, "CTaskFlightlessBirdLocomotion"},
	    {278, "CTaskFishLocomotion"},
	    {279, "CTaskQuadLocomotion"},
	    {280, "CTaskMotionDiving"},
	    {281, "CTaskMotionSwimming"},
	    {282, "CTaskMotionParachuting"},
	    {283, "CTaskMotionDrunk"},
	    {284, "CTaskRepositionMove"},
	    {285, "CTaskMotionAimingTransition"},
	    {286, "CTaskThrowProjectile"},
	    {287, "CTaskCover"},
	    {288, "CTaskMotionInCover"},
	    {289, "CTaskAimAndThrowProjectile"},
	    {290, "CTaskGun"},
	    {291, "CTaskAimFromGround"},
	    {295, "CTaskAimGunVehicleDriveBy"},
	    {296, "CTaskAimGunScripted"},
	    {298, "CTaskReloadGun"},
	    {299, "CTaskWeaponBlocked"},
	    {300, "CTaskEnterCover"},
	    {301, "CTaskExitCover"},
	    {302, "CTaskAimGunFromCoverIntro"},
	    {303, "CTaskAimGunFromCoverOutro"},
	    {304, "CTaskAimGunBlindFire"},
	    {307, "CTaskCombatClosestTargetInArea"},
	    {308, "CTaskCombatAdditionalTask"},
	    {309, "CTaskInCover"},
	    {313, "CTaskAimSweep"},
	    {318, "CTaskSharkCircle"},
	    {319, "CTaskSharkAttack"},
	    {320, "CTaskAgitated"},
	    {321, "CTaskAgitatedAction"},
	    {322, "CTaskConfront"},
	    {323, "CTaskIntimidate"},
	    {324, "CTaskShove"},
	    {325, "CTaskShoved"},
	    {327, "CTaskCrouchToggle"},
	    {328, "CTaskRevive"},
	    {334, "CTaskParachute"},
	    {335, "CTaskParachuteObject"},
	    {336, "CTaskTakeOffPedVariation"},
	    {339, "CTaskCombatSeekCover"},
	    {341, "CTaskCombatFlank"},
	    {342, "CTaskCombat"},
	    {343, "CTaskCombatMounted"},
	    {344, "CTaskMoveCircle"},
	    {345, "CTaskMoveCombatMounted"},
	    {346, "CTaskSearch"},
	    {347, "CTaskSearchOnFoot"},
	    {348, "CTaskSearchInAutomobile"},
	    {349, "CTaskSearchInBoat"},
	    {350, "CTaskSearchInHeli"},
	    {351, "CTaskThreatResponse"},
	    {352, "CTaskInvestigate"},
	    {353, "CTaskStandGuardFSM"},
	    {354, "CTaskPatrol"},
	    {355, "CTaskShootAtTarget"},
	    {356, "CTaskSetAndGuardArea"},
	    {357, "CTaskStandGuard"},
	    {358, "CTaskSeparate"},
	    {359, "CTaskStayInCover"},
	    {360, "CTaskVehicleCombat"},
	    {361, "CTaskVehiclePersuit"},
	    {362, "CTaskVehicleChase"},
	    {363, "CTaskDraggingToSafety"},
	    {364, "CTaskDraggedToSafety"},
	    {365, "CTaskVariedAimPose"},
	    {366, "CTaskMoveWithinAttackWindow"},
	    {367, "CTaskMoveWithinDefensiveArea"},
	    {368, "CTaskShootOutTire"},
	    {369, "CTaskShellShocked"},
	    {370, "CTaskBoatChase"},
	    {371, "CTaskBoatCombat"},
	    {372, "CTaskBoatStrafe"},
	    {373, "CTaskHeliChase"},
	    {374, "CTaskHeliCombat"},
	    {375, "CTaskSubmarineCombat"},
	    {376, "CTaskSubmarineChase"},
	    {377, "CTaskPlaneChase"},
	    {378, "CTaskTargetUnreachable"},
	    {379, "CTaskTargetUnreachableInInterior"},
	    {380, "CTaskTargetUnreachableInExterior"},
	    {381, "CTaskStealthKill"},
	    {382, "CTaskWrithe"},
	    {383, "CTaskAdvance"},
	    {384, "CTaskCharge"},
	    {385, "CTaskMoveToTacticalPoint"},
	    {386, "CTaskToHurtTransit"},
	    {387, "CTaskAnimatedHitByExplosion"},
	    {388, "CTaskNMRelax"},
	    {390, "CTaskNMPose"},
	    {391, "CTaskNMBrace"},
	    {392, "CTaskNMBuoyancy"},
	    {393, "CTaskNMInjuredOnGround"},
	    {394, "CTaskNMShot"},
	    {395, "CTaskNMHighFall"},
	    {396, "CTaskNMBalance"},
	    {397, "CTaskNMElectrocute"},
	    {398, "CTaskNMPrototype"},
	    {399, "CTaskNMExplosion"},
	    {400, "CTaskNMOnFire"},
	    {401, "CTaskNMScriptControl"},
	    {402, "CTaskNMJumpRollFromRoadVehicle"},
	    {403, "CTaskNMFlinch"},
	    {404, "CTaskNMSit"},
	    {405, "CTaskNMFallDown"},
	    {406, "CTaskBlendFromNM"},
	    {407, "CTaskNMControl"},
	    {408, "CTaskNMDangle"},
	    {411, "CTaskNMGenericAttach"},
	    {412, "CTaskNMDrunk"},
	    {413, "CTaskNMDraggingToSafety"},
	    {414, "CTaskNMThroughWindscreen"},
	    {415, "CTaskNMRiverRapids"},
	    {416, "CTaskNMSimple"},
	    {417, "CTaskRageRagdoll"},
	    {420, "CTaskJumpVault"},
	    {421, "CTaskJump"},
	    {422, "CTaskFall"},
	    {424, "CTaskReactAimWeapon"},
	    {425, "CTaskChat"},
	    {426, "CTaskMobilePhone"},
	    {427, "CTaskReactToDeadPed"},
	    {429, "CTaskSearchForUnknownThreat"},
	    {431, "CTaskBomb"},
	    {432, "CTaskDetonator"},
	    {434, "CTaskAnimatedAttach"},
	    {440, "CTaskCutScene"},
	    {441, "CTaskReactToExplosion"},
	    {442, "CTaskReactToImminentExplosion"},
	    {443, "CTaskDiveToGround"},
	    {444, "CTaskReactAndFlee"},
	    {445, "CTaskSidestep"},
	    {446, "CTaskCallPolice"},
	    {447, "CTaskReactInDirection"},
	    {448, "CTaskReactToBuddyShot"},
	    {453, "CTaskVehicleGoToAutomobileNew"},
	    {454, "CTaskVehicleGoToPlane"},
	    {455, "CTaskVehicleGoToHelicopter"},
	    {456, "CTaskVehicleGoToSubmarine"},
	    {457, "CTaskVehicleGoToBoat"},
	    {458, "CTaskVehicleGoToPointAutomobile"},
	    {459, "CTaskVehicleGoToPointWithAvoidanceAutomobile"},
	    {460, "CTaskVehiclePursue"},
	    {461, "CTaskVehicleRam"},
	    {462, "CTaskVehicleSpinOut"},
	    {463, "CTaskVehicleApproach"},
	    {464, "CTaskVehicleThreePointTurn"},
	    {465, "CTaskVehicleDeadDriver"},
	    {466, "CTaskVehicleCruiseNew"},
	    {467, "CTaskVehicleCruiseBoat"},
	    {468, "CTaskVehicleStop"},
	    {469, "CTaskVehiclePullOver"},
	    {470, "CTaskVehiclePassengerExit"},
	    {471, "CTaskVehicleFlee"},
	    {472, "CTaskVehicleFleeAirborne"},
	    {473, "CTaskVehicleFleeBoat"},
	    {474, "CTaskVehicleFollowRecording"},
	    {475, "CTaskVehicleFollow"},
	    {476, "CTaskVehicleBlock"},
	    {477, "CTaskVehicleBlockCruiseInFront"},
	    {478, "CTaskVehicleBlockBrakeInFront"},
	    {479, "CTaskVehicleBlockBackAndForth"},
	    {480, "CTaskVehicleCrash"},
	    {481, "CTaskVehicleLand"},
	    {482, "CTaskVehicleLandPlane"},
	    {483, "CTaskVehicleHover"},
	    {484, "CTaskVehicleAttack"},
	    {485, "CTaskVehicleAttackTank"},
	    {486, "CTaskVehicleCircle"},
	    {487, "CTaskVehiclePoliceBehaviour"},
	    {488, "CTaskVehiclePoliceBehaviourHelicopter"},
	    {489, "CTaskVehiclePoliceBehaviourBoat"},
	    {490, "CTaskVehicleEscort"},
	    {491, "CTaskVehicleHeliProtect"},
	    {493, "CTaskVehiclePlayerDriveAutomobile"},
	    {494, "CTaskVehiclePlayerDriveBike"},
	    {495, "CTaskVehiclePlayerDriveBoat"},
	    {496, "CTaskVehiclePlayerDriveSubmarine"},
	    {497, "CTaskVehiclePlayerDriveSubmarineCar"},
	    {498, "CTaskVehiclePlayerDrivePlane"},
	    {499, "CTaskVehiclePlayerDriveHeli"},
	    {500, "CTaskVehiclePlayerDriveAutogyro"},
	    {501, "CTaskVehiclePlayerDriveDiggerArm"},
	    {502, "CTaskVehiclePlayerDriveTrain"},
	    {503, "CTaskVehiclePlaneChase"},
	    {504, "CTaskVehicleNoDriver"},
	    {505, "CTaskVehicleAnimation"},
	    {506, "CTaskVehicleConvertibleRoof"},
	    {507, "CTaskVehicleParkNew"},
	    {508, "CTaskVehicleFollowWaypointRecording"},
	    {509, "CTaskVehicleGoToNavmesh"},
	    {510, "CTaskVehicleReactToCopSiren"},
	    {511, "CTaskVehicleGotoLongRange"},
	    {512, "CTaskVehicleWait"},
	    {513, "CTaskVehicleReverse"},
	    {514, "CTaskVehicleBrake"},
	    {515, "CTaskVehicleHandBrake"},
	    {516, "CTaskVehicleTurn"},
	    {517, "CTaskVehicleGoForward"},
	    {518, "CTaskVehicleSwerve"},
	    {519, "CTaskVehicleFlyDirection"},
	    {520, "CTaskVehicleHeadonCollision"},
	    {521, "CTaskVehicleBoostUseSteeringAngle"},
	    {522, "CTaskVehicleShotTire"},
	    {523, "CTaskVehicleBurnout"},
	    {524, "CTaskVehicleRevEngine"},
	    {525, "CTaskVehicleSurfaceInSubmarine"},
	    {526, "CTaskVehiclePullAlongside"},
	    {527, "CTaskVehicleTransformToSubmarine"},
	    {528, "CTaskAnimatedFallback"},
	};

	inline bool change_player_model(const Hash hash)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}
		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			return false;
		}
		PLAYER::SET_PLAYER_MODEL(self::id, hash);
		self::ped = PLAYER::PLAYER_PED_ID();
		script::get_current()->yield();
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);
		for (int i = 0; i < 12; i++)
		{
			PED::SET_PED_COMPONENT_VARIATION(self::ped, i, PED::GET_PED_DRAWABLE_VARIATION(self::ped, i), PED::GET_PED_TEXTURE_VARIATION(self::ped, i), PED::GET_PED_PALETTE_VARIATION(self::ped, i));
		}
		return true;
	}

	inline bool steal_outfit(const Ped target)
	{
		Ped ped = self::ped;

		if (ENTITY::GET_ENTITY_MODEL(ped) != ENTITY::GET_ENTITY_MODEL(target))
		{
			return false;
		}
		for (int i = 0; i < 12; i++)
		{
			PED::SET_PED_COMPONENT_VARIATION(ped, i, PED::GET_PED_DRAWABLE_VARIATION(target, i), PED::GET_PED_TEXTURE_VARIATION(target, i), PED::GET_PED_PALETTE_VARIATION(target, i));
		}

		return true;
	}

	inline int spawn_in_vehicle(std::string_view model, Vehicle veh, bool is_networked = true, bool clean_up = true)
	{
		if (const Hash hash = rage::joaat(model.data()); hash)
		{
			for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
			{
				STREAMING::REQUEST_MODEL(hash);

				script::get_current()->yield();
			}
			if (!STREAMING::HAS_MODEL_LOADED(hash))
			{
				g_notification_service->push_warning("Spawn", "Failed to spawn model, did you give an incorrect model?");

				return -1;
			}

			Ped ped = PED::CREATE_PED_INSIDE_VEHICLE(veh, 0, hash, -1, is_networked, false);

			script::get_current()->yield();

			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

			if (*g_pointers->m_is_session_started)
			{
				DECORATOR::DECOR_SET_INT(ped, "MPBitset", 0);
				ENTITY::SET_ENTITY_SHOULD_FREEZE_WAITING_ON_COLLISION(ped, clean_up);
			}

			return ped;
		}

		return -1;
	}

	inline void steal_identity(const Ped target)
	{
		const int max_health     = ENTITY::GET_ENTITY_MAX_HEALTH(self::ped);
		const int current_health = ENTITY::GET_ENTITY_HEALTH(self::ped);
		const int current_armor  = PED::GET_PED_ARMOUR(self::ped);

		PLAYER::SET_PLAYER_MODEL(self::id, ENTITY::GET_ENTITY_MODEL(target));
		script::get_current()->yield();
		PED::CLONE_PED_TO_TARGET(target, self::ped);
		ENTITY::SET_ENTITY_MAX_HEALTH(self::ped, max_health);
		ENTITY::SET_ENTITY_HEALTH(self::ped, current_health, 0);
		PED::SET_PED_ARMOUR(self::ped, current_armor);
	}

	inline void kill_ped(const Ped ped)
	{
		if (entity::take_control_of(ped))
			PED::APPLY_DAMAGE_TO_PED(ped, PED::GET_PED_MAX_HEALTH(ped) * 2, false, 0);
	}

	inline void kill_ped_by_relation(Ped ped, int relation_id)
	{
		if (PED::GET_RELATIONSHIP_BETWEEN_PEDS(ped, PLAYER::PLAYER_PED_ID()) == relation_id)
			kill_ped(ped);
	}

	inline Ped spawn(ePedType pedType, Hash hash, Hash clone, Vector3 location, float heading, bool is_networked = true)
	{
		for (uint8_t i = 0; !STREAMING::HAS_MODEL_LOADED(hash) && i < 100; i++)
		{
			STREAMING::REQUEST_MODEL(hash);
			script::get_current()->yield();
		}

		if (!STREAMING::HAS_MODEL_LOADED(hash))
		{
			return 0;
		}

		auto ped = PED::CREATE_PED(pedType, hash, location.x, location.y, location.z, heading, is_networked, false);

		script::get_current()->yield();

		if (clone)
		{
			PED::CLONE_PED_TO_TARGET(clone, ped);
		}

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(hash);

		return ped;
	}

	///
	/// GPL-3 CODE STARTS HERE
	/// SKIDDED FROM https://github.com/gta-chaos-mod/ChaosModV
	///

	inline Hash create_bad_ped_relationship_group(std::string group_name)
	{
		static const Hash playerGroup = RAGE_JOAAT("PLAYER");
		static const Hash civGroup    = RAGE_JOAAT("CIVMALE");
		static const Hash femCivGroup = RAGE_JOAAT("CIVFEMALE");

		Hash relationshipGroup;
		PED::ADD_RELATIONSHIP_GROUP(group_name.c_str(), &relationshipGroup);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, playerGroup);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, civGroup);
		PED::SET_RELATIONSHIP_BETWEEN_GROUPS(5, relationshipGroup, femCivGroup);

		return relationshipGroup;
	}

	inline Ped spawn_griefer_jesus(Vector3 pos, Ped target)
	{
		Hash relationshipGroup = create_bad_ped_relationship_group("_HOSTILE_JESUS");

		Ped ped = ped::spawn(ePedType::PED_TYPE_CRIMINAL, RAGE_JOAAT("u_m_m_jesus_01"), 0, pos, 0);

		if (PED::IS_PED_IN_ANY_VEHICLE(target, false))
		{
			PED::SET_PED_INTO_VEHICLE(ped, PED::GET_VEHICLE_PED_IS_IN(target, false), -2);
		}

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		PED::SET_PED_HEARING_RANGE(ped, 9999.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true);

		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
		PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		TASK::TASK_COMBAT_PED(ped, target, 0, 16);

		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));

		return ped;
	}

	inline Ped spawn_extrime_griefer_jesus(Vector3 pos, Ped target)
	{
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(target, false) ? PED::GET_VEHICLE_PED_IS_IN(target, false) : target);

		Vehicle veh = vehicle::spawn(RAGE_JOAAT("oppressor2"), pos, heading, true, false, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		vehicle::max_vehicle(veh);
		ENTITY::SET_ENTITY_PROOFS(veh, false, true, true, false, false, false, false, false);

		Hash relationshipGroup = create_bad_ped_relationship_group("_HOSTILE_JESUS");

		Ped ped = ped::spawn_in_vehicle("u_m_m_jesus_01", veh, true);

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		PED::SET_PED_HEARING_RANGE(ped, 9999.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true);

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		ENTITY::SET_ENTITY_PROOFS(ped, false, true, true, false, false, false, false, false);

		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, 1);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_RAGDOLL_BLOCKING_FLAGS(ped, 5);
		PED::SET_PED_SUFFERS_CRITICAL_HITS(ped, false);

		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		TASK::TASK_COMBAT_PED(ped, target, 0, 16);

		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));

		return ped;
	}

	inline Ped spawn_griefer_jet(Vector3 pos, Ped target, Hash vehicle)
	{
		float heading = ENTITY::GET_ENTITY_HEADING(PED::IS_PED_IN_ANY_VEHICLE(target, false) ? PED::GET_VEHICLE_PED_IS_IN(target, false) : target);

		Vehicle veh = vehicle::spawn(vehicle, pos, heading, true);
		VEHICLE::SET_VEHICLE_ENGINE_ON(veh, true, true, false);
		VEHICLE::CONTROL_LANDING_GEAR(veh, 3);

		Hash relationshipGroup = create_bad_ped_relationship_group("_HOSTILE_JESUS");

		Ped ped = ped::spawn_in_vehicle("u_m_m_jesus_01", veh, true);

		PED::SET_PED_RELATIONSHIP_GROUP_HASH(ped, relationshipGroup);
		PED::SET_PED_HEARING_RANGE(ped, 9999.f);
		PED::SET_PED_CONFIG_FLAG(ped, 281, true);

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 3, false);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 46, true);

		TASK::TASK_PLANE_MISSION(ped, veh, 0, target, 0, 0, 0, 6, 0.0, 0.0, 0.0, 2500.0, -1500.0, 0);

		WEAPON::GIVE_WEAPON_TO_PED(ped, RAGE_JOAAT("WEAPON_RAILGUN"), 9999, true, true);
		TASK::TASK_COMBAT_PED(ped, target, 0, 16);

		PED::SET_PED_FIRING_PATTERN(ped, RAGE_JOAAT("FIRING_PATTERN_FULL_AUTO"));

		return ped;
	}

	///
	/// GPL-3 CODE ENDS HERE
	///

	inline void set_ped_random_component_variation(Ped ped)
	{
		auto range = [](int lower_bound, int upper_bound) -> int {
			return std::rand() % (upper_bound - lower_bound + 1) + lower_bound;
		};
		outfit::components_t components;
		for (auto& item : components.items)
		{
			int drawable_id_max = PED::GET_NUMBER_OF_PED_DRAWABLE_VARIATIONS(ped, item.id) - 1;
			if (drawable_id_max == -1)
				continue;
			int drawable_id = range(0, drawable_id_max);
			int texture_id_max = PED::GET_NUMBER_OF_PED_TEXTURE_VARIATIONS(ped, item.id, drawable_id) - 1;
			if (texture_id_max == -1)
				continue;
			int texture_id = range(0, texture_id_max);
			PED::SET_PED_COMPONENT_VARIATION(ped, item.id, drawable_id, texture_id, PED::GET_PED_PALETTE_VARIATION(ped, item.id));
		}
	}

	inline player_ptr get_player_from_ped(Ped ped)
	{
		for (auto& p : g_player_service->players())
		{
			if (p.second->get_ped())
			{
				if (p.second->get_ped() == g_pointers->m_handle_to_ptr(ped))
					return p.second;
			}
		}
		return nullptr;
	}

	inline bool load_animation_dict (const char* dict)
	{
		if (STREAMING::HAS_ANIM_DICT_LOADED(dict))
			return true;

		for (uint8_t i = 0; !STREAMING::HAS_ANIM_DICT_LOADED(dict) && i < 35; i++)
		{
			STREAMING::REQUEST_ANIM_DICT(dict);
			script::get_current()->yield();
		}

		return STREAMING::HAS_ANIM_DICT_LOADED(dict);
	}

	inline void ped_play_animation(Ped ped, const std::string_view& animDict, const std::string_view& animName, float speed = 4.f, float speedMultiplier = -4.f, int duration = -1, int flag = 0, float playbackRate = 0, bool lockPos = false)
	{
		if (load_animation_dict(animDict.data()))
			TASK::TASK_PLAY_ANIM(ped, animDict.data(), animName.data(), speed, speedMultiplier, duration, flag, playbackRate, lockPos, lockPos, lockPos);		
	}

	/*
	* Will make the ped enter the vehicle with animation if vehicle is in vicinity
	* Param movespeed: 1 = walk, 2 = run, 3 = sprint
	*/
	inline void ped_enter_vehicle_animated(Ped ped, Vehicle veh, eVehicleSeats seat, int movespeed)
	{
		if (entity::take_control_of(ped))
		{
			if (ENTITY::DOES_ENTITY_EXIST(veh)) {
				if (math::distance_between_vectors(ENTITY::GET_ENTITY_COORDS(ped, 0), ENTITY::GET_ENTITY_COORDS(veh, 0)) < 15.f)
					TASK::TASK_ENTER_VEHICLE(ped, veh, 10000, (int)seat, movespeed, 8, NULL);
				else
					PED::SET_PED_INTO_VEHICLE(ped, veh, (int)seat);
			}
		}
	}

}
