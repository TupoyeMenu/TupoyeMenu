#if defined (ENABLE_LUA)

#include "lua_native_binding.hpp"
#include "natives.hpp"

namespace lua::native
{
	static void LUA_NATIVE_STREAMING_LOAD_ALL_OBJECTS_NOW()
	{
		STREAMING::LOAD_ALL_OBJECTS_NOW();
	}

	static void LUA_NATIVE_STREAMING_LOAD_SCENE(float x, float y, float z)
	{
		STREAMING::LOAD_SCENE(x, y, z);
	}

	static bool LUA_NATIVE_STREAMING_NETWORK_UPDATE_LOAD_SCENE()
	{
		auto retval = (bool)STREAMING::NETWORK_UPDATE_LOAD_SCENE();
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_NETWORK_LOADING_SCENE()
	{
		auto retval = (bool)STREAMING::IS_NETWORK_LOADING_SCENE();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_INTERIOR_ACTIVE(int interiorID, bool toggle)
	{
		STREAMING::SET_INTERIOR_ACTIVE(interiorID, toggle);
	}

	static void LUA_NATIVE_STREAMING_REQUEST_MODEL(Hash model)
	{
		STREAMING::REQUEST_MODEL(model);
	}

	static void LUA_NATIVE_STREAMING_REQUEST_MENU_PED_MODEL(Hash model)
	{
		STREAMING::REQUEST_MENU_PED_MODEL(model);
	}

	static bool LUA_NATIVE_STREAMING_HAS_MODEL_LOADED(Hash model)
	{
		auto retval = (bool)STREAMING::HAS_MODEL_LOADED(model);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REQUEST_MODELS_IN_ROOM(Interior interior, const char* roomName)
	{
		STREAMING::REQUEST_MODELS_IN_ROOM(interior, roomName);
	}

	static void LUA_NATIVE_STREAMING_SET_MODEL_AS_NO_LONGER_NEEDED(Hash model)
	{
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	static bool LUA_NATIVE_STREAMING_IS_MODEL_IN_CDIMAGE(Hash model)
	{
		auto retval = (bool)STREAMING::IS_MODEL_IN_CDIMAGE(model);
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_MODEL_VALID(Hash model)
	{
		auto retval = (bool)STREAMING::IS_MODEL_VALID(model);
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_MODEL_A_PED(Hash model)
	{
		auto retval = (bool)STREAMING::IS_MODEL_A_PED(model);
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_MODEL_A_VEHICLE(Hash model)
	{
		auto retval = (bool)STREAMING::IS_MODEL_A_VEHICLE(model);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REQUEST_COLLISION_AT_COORD(float x, float y, float z)
	{
		STREAMING::REQUEST_COLLISION_AT_COORD(x, y, z);
	}

	static void LUA_NATIVE_STREAMING_REQUEST_COLLISION_FOR_MODEL(Hash model)
	{
		STREAMING::REQUEST_COLLISION_FOR_MODEL(model);
	}

	static bool LUA_NATIVE_STREAMING_HAS_COLLISION_FOR_MODEL_LOADED(Hash model)
	{
		auto retval = (bool)STREAMING::HAS_COLLISION_FOR_MODEL_LOADED(model);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REQUEST_ADDITIONAL_COLLISION_AT_COORD(float x, float y, float z)
	{
		STREAMING::REQUEST_ADDITIONAL_COLLISION_AT_COORD(x, y, z);
	}

	static bool LUA_NATIVE_STREAMING_DOES_ANIM_DICT_EXIST(const char* animDict)
	{
		auto retval = (bool)STREAMING::DOES_ANIM_DICT_EXIST(animDict);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REQUEST_ANIM_DICT(const char* animDict)
	{
		STREAMING::REQUEST_ANIM_DICT(animDict);
	}

	static bool LUA_NATIVE_STREAMING_HAS_ANIM_DICT_LOADED(const char* animDict)
	{
		auto retval = (bool)STREAMING::HAS_ANIM_DICT_LOADED(animDict);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REMOVE_ANIM_DICT(const char* animDict)
	{
		STREAMING::REMOVE_ANIM_DICT(animDict);
	}

	static void LUA_NATIVE_STREAMING_REQUEST_ANIM_SET(const char* animSet)
	{
		STREAMING::REQUEST_ANIM_SET(animSet);
	}

	static bool LUA_NATIVE_STREAMING_HAS_ANIM_SET_LOADED(const char* animSet)
	{
		auto retval = (bool)STREAMING::HAS_ANIM_SET_LOADED(animSet);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REMOVE_ANIM_SET(const char* animSet)
	{
		STREAMING::REMOVE_ANIM_SET(animSet);
	}

	static void LUA_NATIVE_STREAMING_REQUEST_CLIP_SET(const char* clipSet)
	{
		STREAMING::REQUEST_CLIP_SET(clipSet);
	}

	static bool LUA_NATIVE_STREAMING_HAS_CLIP_SET_LOADED(const char* clipSet)
	{
		auto retval = (bool)STREAMING::HAS_CLIP_SET_LOADED(clipSet);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REMOVE_CLIP_SET(const char* clipSet)
	{
		STREAMING::REMOVE_CLIP_SET(clipSet);
	}

	static void LUA_NATIVE_STREAMING_REQUEST_IPL(const char* iplName)
	{
		STREAMING::REQUEST_IPL(iplName);
	}

	static void LUA_NATIVE_STREAMING_REMOVE_IPL(const char* iplName)
	{
		STREAMING::REMOVE_IPL(iplName);
	}

	static bool LUA_NATIVE_STREAMING_IS_IPL_ACTIVE(const char* iplName)
	{
		auto retval = (bool)STREAMING::IS_IPL_ACTIVE(iplName);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_STREAMING(bool toggle)
	{
		STREAMING::SET_STREAMING(toggle);
	}

	static void LUA_NATIVE_STREAMING_LOAD_GLOBAL_WATER_FILE(int waterType)
	{
		STREAMING::LOAD_GLOBAL_WATER_FILE(waterType);
	}

	static int LUA_NATIVE_STREAMING_GET_GLOBAL_WATER_FILE()
	{
		auto retval = STREAMING::GET_GLOBAL_WATER_FILE();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_GAME_PAUSES_FOR_STREAMING(bool toggle)
	{
		STREAMING::SET_GAME_PAUSES_FOR_STREAMING(toggle);
	}

	static void LUA_NATIVE_STREAMING_SET_REDUCE_PED_MODEL_BUDGET(bool toggle)
	{
		STREAMING::SET_REDUCE_PED_MODEL_BUDGET(toggle);
	}

	static void LUA_NATIVE_STREAMING_SET_REDUCE_VEHICLE_MODEL_BUDGET(bool toggle)
	{
		STREAMING::SET_REDUCE_VEHICLE_MODEL_BUDGET(toggle);
	}

	static void LUA_NATIVE_STREAMING_SET_DITCH_POLICE_MODELS(bool toggle)
	{
		STREAMING::SET_DITCH_POLICE_MODELS(toggle);
	}

	static int LUA_NATIVE_STREAMING_GET_NUMBER_OF_STREAMING_REQUESTS()
	{
		auto retval = STREAMING::GET_NUMBER_OF_STREAMING_REQUESTS();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REQUEST_PTFX_ASSET()
	{
		STREAMING::REQUEST_PTFX_ASSET();
	}

	static bool LUA_NATIVE_STREAMING_HAS_PTFX_ASSET_LOADED()
	{
		auto retval = (bool)STREAMING::HAS_PTFX_ASSET_LOADED();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REMOVE_PTFX_ASSET()
	{
		STREAMING::REMOVE_PTFX_ASSET();
	}

	static void LUA_NATIVE_STREAMING_REQUEST_NAMED_PTFX_ASSET(const char* fxName)
	{
		STREAMING::REQUEST_NAMED_PTFX_ASSET(fxName);
	}

	static bool LUA_NATIVE_STREAMING_HAS_NAMED_PTFX_ASSET_LOADED(const char* fxName)
	{
		auto retval = (bool)STREAMING::HAS_NAMED_PTFX_ASSET_LOADED(fxName);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REMOVE_NAMED_PTFX_ASSET(const char* fxName)
	{
		STREAMING::REMOVE_NAMED_PTFX_ASSET(fxName);
	}

	static void LUA_NATIVE_STREAMING_SET_VEHICLE_POPULATION_BUDGET(int p0)
	{
		STREAMING::SET_VEHICLE_POPULATION_BUDGET(p0);
	}

	static void LUA_NATIVE_STREAMING_SET_PED_POPULATION_BUDGET(int p0)
	{
		STREAMING::SET_PED_POPULATION_BUDGET(p0);
	}

	static void LUA_NATIVE_STREAMING_CLEAR_FOCUS()
	{
		STREAMING::CLEAR_FOCUS();
	}

	static void LUA_NATIVE_STREAMING_SET_FOCUS_POS_AND_VEL(float x, float y, float z, float offsetX, float offsetY, float offsetZ)
	{
		STREAMING::SET_FOCUS_POS_AND_VEL(x, y, z, offsetX, offsetY, offsetZ);
	}

	static void LUA_NATIVE_STREAMING_SET_FOCUS_ENTITY(Entity entity)
	{
		STREAMING::SET_FOCUS_ENTITY(entity);
	}

	static bool LUA_NATIVE_STREAMING_IS_ENTITY_FOCUS(Entity entity)
	{
		auto retval = (bool)STREAMING::IS_ENTITY_FOCUS(entity);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_RESTORE_FOCUS_ENTITY(Entity p0)
	{
		STREAMING::SET_RESTORE_FOCUS_ENTITY(p0);
	}

	static void LUA_NATIVE_STREAMING_SET_MAPDATACULLBOX_ENABLED(const char* name, bool toggle)
	{
		STREAMING::SET_MAPDATACULLBOX_ENABLED(name, toggle);
	}

	static void LUA_NATIVE_STREAMING_SET_ALL_MAPDATA_CULLED(Any p0)
	{
		STREAMING::SET_ALL_MAPDATA_CULLED(p0);
	}

	static int LUA_NATIVE_STREAMING_STREAMVOL_CREATE_SPHERE(float x, float y, float z, float rad, Any p4, Any p5)
	{
		auto retval = STREAMING::STREAMVOL_CREATE_SPHERE(x, y, z, rad, p4, p5);
		return retval;
	}

	static int LUA_NATIVE_STREAMING_STREAMVOL_CREATE_FRUSTUM(float p0, float p1, float p2, float p3, float p4, float p5, float p6, Any p7, Any p8)
	{
		auto retval = STREAMING::STREAMVOL_CREATE_FRUSTUM(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		return retval;
	}

	static int LUA_NATIVE_STREAMING_STREAMVOL_CREATE_LINE(float p0, float p1, float p2, float p3, float p4, float p5, Any p6)
	{
		auto retval = STREAMING::STREAMVOL_CREATE_LINE(p0, p1, p2, p3, p4, p5, p6);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_STREAMVOL_DELETE(Any unused)
	{
		STREAMING::STREAMVOL_DELETE(unused);
	}

	static bool LUA_NATIVE_STREAMING_STREAMVOL_HAS_LOADED(Any unused)
	{
		auto retval = (bool)STREAMING::STREAMVOL_HAS_LOADED(unused);
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_STREAMVOL_IS_VALID(Any unused)
	{
		auto retval = (bool)STREAMING::STREAMVOL_IS_VALID(unused);
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_STREAMVOL_ACTIVE()
	{
		auto retval = (bool)STREAMING::IS_STREAMVOL_ACTIVE();
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_NEW_LOAD_SCENE_START(float posX, float posY, float posZ, float offsetX, float offsetY, float offsetZ, float radius, int p7)
	{
		auto retval = (bool)STREAMING::NEW_LOAD_SCENE_START(posX, posY, posZ, offsetX, offsetY, offsetZ, radius, p7);
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_NEW_LOAD_SCENE_START_SPHERE(float x, float y, float z, float radius, Any p4)
	{
		auto retval = (bool)STREAMING::NEW_LOAD_SCENE_START_SPHERE(x, y, z, radius, p4);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_NEW_LOAD_SCENE_STOP()
	{
		STREAMING::NEW_LOAD_SCENE_STOP();
	}

	static bool LUA_NATIVE_STREAMING_IS_NEW_LOAD_SCENE_ACTIVE()
	{
		auto retval = (bool)STREAMING::IS_NEW_LOAD_SCENE_ACTIVE();
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_NEW_LOAD_SCENE_LOADED()
	{
		auto retval = (bool)STREAMING::IS_NEW_LOAD_SCENE_LOADED();
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_SAFE_TO_START_PLAYER_SWITCH()
	{
		auto retval = (bool)STREAMING::IS_SAFE_TO_START_PLAYER_SWITCH();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_START_PLAYER_SWITCH(Ped from, Ped to, int flags, int switchType)
	{
		STREAMING::START_PLAYER_SWITCH(from, to, flags, switchType);
	}

	static void LUA_NATIVE_STREAMING_STOP_PLAYER_SWITCH()
	{
		STREAMING::STOP_PLAYER_SWITCH();
	}

	static bool LUA_NATIVE_STREAMING_IS_PLAYER_SWITCH_IN_PROGRESS()
	{
		auto retval = (bool)STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS();
		return retval;
	}

	static int LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_TYPE()
	{
		auto retval = STREAMING::GET_PLAYER_SWITCH_TYPE();
		return retval;
	}

	static int LUA_NATIVE_STREAMING_GET_IDEAL_PLAYER_SWITCH_TYPE(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		auto retval = STREAMING::GET_IDEAL_PLAYER_SWITCH_TYPE(x1, y1, z1, x2, y2, z2);
		return retval;
	}

	static int LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_STATE()
	{
		auto retval = STREAMING::GET_PLAYER_SWITCH_STATE();
		return retval;
	}

	static int LUA_NATIVE_STREAMING_GET_PLAYER_SHORT_SWITCH_STATE()
	{
		auto retval = STREAMING::GET_PLAYER_SHORT_SWITCH_STATE();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_PLAYER_SHORT_SWITCH_STYLE(int p0)
	{
		STREAMING::SET_PLAYER_SHORT_SWITCH_STYLE(p0);
	}

	static int LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_JUMP_CUT_INDEX()
	{
		auto retval = STREAMING::GET_PLAYER_SWITCH_JUMP_CUT_INDEX();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_PLAYER_SWITCH_OUTRO(float cameraCoordX, float cameraCoordY, float cameraCoordZ, float camRotationX, float camRotationY, float camRotationZ, float camFov, float camFarClip, int rotationOrder)
	{
		STREAMING::SET_PLAYER_SWITCH_OUTRO(cameraCoordX, cameraCoordY, cameraCoordZ, camRotationX, camRotationY, camRotationZ, camFov, camFarClip, rotationOrder);
	}

	static void LUA_NATIVE_STREAMING_SET_PLAYER_SWITCH_ESTABLISHING_SHOT(const char* name)
	{
		STREAMING::SET_PLAYER_SWITCH_ESTABLISHING_SHOT(name);
	}

	static void LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_PAN()
	{
		STREAMING::ALLOW_PLAYER_SWITCH_PAN();
	}

	static void LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_OUTRO()
	{
		STREAMING::ALLOW_PLAYER_SWITCH_OUTRO();
	}

	static void LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_ASCENT()
	{
		STREAMING::ALLOW_PLAYER_SWITCH_ASCENT();
	}

	static void LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_DESCENT()
	{
		STREAMING::ALLOW_PLAYER_SWITCH_DESCENT();
	}

	static bool LUA_NATIVE_STREAMING_IS_SWITCH_READY_FOR_DESCENT()
	{
		auto retval = (bool)STREAMING::IS_SWITCH_READY_FOR_DESCENT();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_ENABLE_SWITCH_PAUSE_BEFORE_DESCENT()
	{
		STREAMING::ENABLE_SWITCH_PAUSE_BEFORE_DESCENT();
	}

	static void LUA_NATIVE_STREAMING_DISABLE_SWITCH_OUTRO_FX()
	{
		STREAMING::DISABLE_SWITCH_OUTRO_FX();
	}

	static void LUA_NATIVE_STREAMING_SWITCH_TO_MULTI_FIRSTPART(Ped ped, int flags, int switchType)
	{
		STREAMING::SWITCH_TO_MULTI_FIRSTPART(ped, flags, switchType);
	}

	static void LUA_NATIVE_STREAMING_SWITCH_TO_MULTI_SECONDPART(Ped ped)
	{
		STREAMING::SWITCH_TO_MULTI_SECONDPART(ped);
	}

	static bool LUA_NATIVE_STREAMING_IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED()
	{
		auto retval = (bool)STREAMING::IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED();
		return retval;
	}

	static int LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_INTERP_OUT_DURATION()
	{
		auto retval = STREAMING::GET_PLAYER_SWITCH_INTERP_OUT_DURATION();
		return retval;
	}

	static int LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_INTERP_OUT_CURRENT_TIME()
	{
		auto retval = STREAMING::GET_PLAYER_SWITCH_INTERP_OUT_CURRENT_TIME();
		return retval;
	}

	static bool LUA_NATIVE_STREAMING_IS_SWITCH_SKIPPING_DESCENT()
	{
		auto retval = (bool)STREAMING::IS_SWITCH_SKIPPING_DESCENT();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_SCENE_STREAMING_TRACKS_CAM_POS_THIS_FRAME()
	{
		STREAMING::SET_SCENE_STREAMING_TRACKS_CAM_POS_THIS_FRAME();
	}

	static float LUA_NATIVE_STREAMING_GET_LODSCALE()
	{
		auto retval = STREAMING::GET_LODSCALE();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_OVERRIDE_LODSCALE_THIS_FRAME(float scaling)
	{
		STREAMING::OVERRIDE_LODSCALE_THIS_FRAME(scaling);
	}

	static void LUA_NATIVE_STREAMING_REMAP_LODSCALE_RANGE_THIS_FRAME(float p0, float p1, float p2, float p3)
	{
		STREAMING::REMAP_LODSCALE_RANGE_THIS_FRAME(p0, p1, p2, p3);
	}

	static void LUA_NATIVE_STREAMING_SUPPRESS_HD_MAP_STREAMING_THIS_FRAME()
	{
		STREAMING::SUPPRESS_HD_MAP_STREAMING_THIS_FRAME();
	}

	static void LUA_NATIVE_STREAMING_SET_RENDER_HD_ONLY(bool toggle)
	{
		STREAMING::SET_RENDER_HD_ONLY(toggle);
	}

	static void LUA_NATIVE_STREAMING_FORCE_ALLOW_TIME_BASED_FADING_THIS_FRAME()
	{
		STREAMING::FORCE_ALLOW_TIME_BASED_FADING_THIS_FRAME();
	}

	static void LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_START(const char* iplName1, const char* iplName2)
	{
		STREAMING::IPL_GROUP_SWAP_START(iplName1, iplName2);
	}

	static void LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_CANCEL()
	{
		STREAMING::IPL_GROUP_SWAP_CANCEL();
	}

	static bool LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_IS_READY()
	{
		auto retval = (bool)STREAMING::IPL_GROUP_SWAP_IS_READY();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_FINISH()
	{
		STREAMING::IPL_GROUP_SWAP_FINISH();
	}

	static bool LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_IS_ACTIVE()
	{
		auto retval = (bool)STREAMING::IPL_GROUP_SWAP_IS_ACTIVE();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_PREFETCH_SRL(const char* srl)
	{
		STREAMING::PREFETCH_SRL(srl);
	}

	static bool LUA_NATIVE_STREAMING_IS_SRL_LOADED()
	{
		auto retval = (bool)STREAMING::IS_SRL_LOADED();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_BEGIN_SRL()
	{
		STREAMING::BEGIN_SRL();
	}

	static void LUA_NATIVE_STREAMING_END_SRL()
	{
		STREAMING::END_SRL();
	}

	static void LUA_NATIVE_STREAMING_SET_SRL_TIME(float p0)
	{
		STREAMING::SET_SRL_TIME(p0);
	}

	static void LUA_NATIVE_STREAMING_SET_SRL_POST_CUTSCENE_CAMERA(Any p0, Any p1, Any p2, Any p3, Any p4, Any p5)
	{
		STREAMING::SET_SRL_POST_CUTSCENE_CAMERA(p0, p1, p2, p3, p4, p5);
	}

	static void LUA_NATIVE_STREAMING_SET_SRL_READAHEAD_TIMES(Any p0, Any p1, Any p2, Any p3)
	{
		STREAMING::SET_SRL_READAHEAD_TIMES(p0, p1, p2, p3);
	}

	static void LUA_NATIVE_STREAMING_SET_SRL_LONG_JUMP_MODE(bool p0)
	{
		STREAMING::SET_SRL_LONG_JUMP_MODE(p0);
	}

	static void LUA_NATIVE_STREAMING_SET_SRL_FORCE_PRESTREAM(Any p0)
	{
		STREAMING::SET_SRL_FORCE_PRESTREAM(p0);
	}

	static void LUA_NATIVE_STREAMING_SET_HD_AREA(float x, float y, float z, float radius)
	{
		STREAMING::SET_HD_AREA(x, y, z, radius);
	}

	static void LUA_NATIVE_STREAMING_CLEAR_HD_AREA()
	{
		STREAMING::CLEAR_HD_AREA();
	}

	static void LUA_NATIVE_STREAMING_INIT_CREATOR_BUDGET()
	{
		STREAMING::INIT_CREATOR_BUDGET();
	}

	static void LUA_NATIVE_STREAMING_SHUTDOWN_CREATOR_BUDGET()
	{
		STREAMING::SHUTDOWN_CREATOR_BUDGET();
	}

	static bool LUA_NATIVE_STREAMING_ADD_MODEL_TO_CREATOR_BUDGET(Hash modelHash)
	{
		auto retval = (bool)STREAMING::ADD_MODEL_TO_CREATOR_BUDGET(modelHash);
		return retval;
	}

	static void LUA_NATIVE_STREAMING_REMOVE_MODEL_FROM_CREATOR_BUDGET(Hash modelHash)
	{
		STREAMING::REMOVE_MODEL_FROM_CREATOR_BUDGET(modelHash);
	}

	static float LUA_NATIVE_STREAMING_GET_USED_CREATOR_BUDGET()
	{
		auto retval = STREAMING::GET_USED_CREATOR_BUDGET();
		return retval;
	}

	static void LUA_NATIVE_STREAMING_SET_ISLAND_ENABLED(const char* name, bool toggle)
	{
		STREAMING::SET_ISLAND_ENABLED(name, toggle);
	}

	void init_native_binding_STREAMING(sol::state& L)
	{
		auto STREAMING = L["STREAMING"].get_or_create<sol::table>();
		STREAMING.set_function("LOAD_ALL_OBJECTS_NOW", LUA_NATIVE_STREAMING_LOAD_ALL_OBJECTS_NOW);
		STREAMING.set_function("LOAD_SCENE", LUA_NATIVE_STREAMING_LOAD_SCENE);
		STREAMING.set_function("NETWORK_UPDATE_LOAD_SCENE", LUA_NATIVE_STREAMING_NETWORK_UPDATE_LOAD_SCENE);
		STREAMING.set_function("IS_NETWORK_LOADING_SCENE", LUA_NATIVE_STREAMING_IS_NETWORK_LOADING_SCENE);
		STREAMING.set_function("SET_INTERIOR_ACTIVE", LUA_NATIVE_STREAMING_SET_INTERIOR_ACTIVE);
		STREAMING.set_function("REQUEST_MODEL", LUA_NATIVE_STREAMING_REQUEST_MODEL);
		STREAMING.set_function("REQUEST_MENU_PED_MODEL", LUA_NATIVE_STREAMING_REQUEST_MENU_PED_MODEL);
		STREAMING.set_function("HAS_MODEL_LOADED", LUA_NATIVE_STREAMING_HAS_MODEL_LOADED);
		STREAMING.set_function("REQUEST_MODELS_IN_ROOM", LUA_NATIVE_STREAMING_REQUEST_MODELS_IN_ROOM);
		STREAMING.set_function("SET_MODEL_AS_NO_LONGER_NEEDED", LUA_NATIVE_STREAMING_SET_MODEL_AS_NO_LONGER_NEEDED);
		STREAMING.set_function("IS_MODEL_IN_CDIMAGE", LUA_NATIVE_STREAMING_IS_MODEL_IN_CDIMAGE);
		STREAMING.set_function("IS_MODEL_VALID", LUA_NATIVE_STREAMING_IS_MODEL_VALID);
		STREAMING.set_function("IS_MODEL_A_PED", LUA_NATIVE_STREAMING_IS_MODEL_A_PED);
		STREAMING.set_function("IS_MODEL_A_VEHICLE", LUA_NATIVE_STREAMING_IS_MODEL_A_VEHICLE);
		STREAMING.set_function("REQUEST_COLLISION_AT_COORD", LUA_NATIVE_STREAMING_REQUEST_COLLISION_AT_COORD);
		STREAMING.set_function("REQUEST_COLLISION_FOR_MODEL", LUA_NATIVE_STREAMING_REQUEST_COLLISION_FOR_MODEL);
		STREAMING.set_function("HAS_COLLISION_FOR_MODEL_LOADED", LUA_NATIVE_STREAMING_HAS_COLLISION_FOR_MODEL_LOADED);
		STREAMING.set_function("REQUEST_ADDITIONAL_COLLISION_AT_COORD", LUA_NATIVE_STREAMING_REQUEST_ADDITIONAL_COLLISION_AT_COORD);
		STREAMING.set_function("DOES_ANIM_DICT_EXIST", LUA_NATIVE_STREAMING_DOES_ANIM_DICT_EXIST);
		STREAMING.set_function("REQUEST_ANIM_DICT", LUA_NATIVE_STREAMING_REQUEST_ANIM_DICT);
		STREAMING.set_function("HAS_ANIM_DICT_LOADED", LUA_NATIVE_STREAMING_HAS_ANIM_DICT_LOADED);
		STREAMING.set_function("REMOVE_ANIM_DICT", LUA_NATIVE_STREAMING_REMOVE_ANIM_DICT);
		STREAMING.set_function("REQUEST_ANIM_SET", LUA_NATIVE_STREAMING_REQUEST_ANIM_SET);
		STREAMING.set_function("HAS_ANIM_SET_LOADED", LUA_NATIVE_STREAMING_HAS_ANIM_SET_LOADED);
		STREAMING.set_function("REMOVE_ANIM_SET", LUA_NATIVE_STREAMING_REMOVE_ANIM_SET);
		STREAMING.set_function("REQUEST_CLIP_SET", LUA_NATIVE_STREAMING_REQUEST_CLIP_SET);
		STREAMING.set_function("HAS_CLIP_SET_LOADED", LUA_NATIVE_STREAMING_HAS_CLIP_SET_LOADED);
		STREAMING.set_function("REMOVE_CLIP_SET", LUA_NATIVE_STREAMING_REMOVE_CLIP_SET);
		STREAMING.set_function("REQUEST_IPL", LUA_NATIVE_STREAMING_REQUEST_IPL);
		STREAMING.set_function("REMOVE_IPL", LUA_NATIVE_STREAMING_REMOVE_IPL);
		STREAMING.set_function("IS_IPL_ACTIVE", LUA_NATIVE_STREAMING_IS_IPL_ACTIVE);
		STREAMING.set_function("SET_STREAMING", LUA_NATIVE_STREAMING_SET_STREAMING);
		STREAMING.set_function("LOAD_GLOBAL_WATER_FILE", LUA_NATIVE_STREAMING_LOAD_GLOBAL_WATER_FILE);
		STREAMING.set_function("GET_GLOBAL_WATER_FILE", LUA_NATIVE_STREAMING_GET_GLOBAL_WATER_FILE);
		STREAMING.set_function("SET_GAME_PAUSES_FOR_STREAMING", LUA_NATIVE_STREAMING_SET_GAME_PAUSES_FOR_STREAMING);
		STREAMING.set_function("SET_REDUCE_PED_MODEL_BUDGET", LUA_NATIVE_STREAMING_SET_REDUCE_PED_MODEL_BUDGET);
		STREAMING.set_function("SET_REDUCE_VEHICLE_MODEL_BUDGET", LUA_NATIVE_STREAMING_SET_REDUCE_VEHICLE_MODEL_BUDGET);
		STREAMING.set_function("SET_DITCH_POLICE_MODELS", LUA_NATIVE_STREAMING_SET_DITCH_POLICE_MODELS);
		STREAMING.set_function("GET_NUMBER_OF_STREAMING_REQUESTS", LUA_NATIVE_STREAMING_GET_NUMBER_OF_STREAMING_REQUESTS);
		STREAMING.set_function("REQUEST_PTFX_ASSET", LUA_NATIVE_STREAMING_REQUEST_PTFX_ASSET);
		STREAMING.set_function("HAS_PTFX_ASSET_LOADED", LUA_NATIVE_STREAMING_HAS_PTFX_ASSET_LOADED);
		STREAMING.set_function("REMOVE_PTFX_ASSET", LUA_NATIVE_STREAMING_REMOVE_PTFX_ASSET);
		STREAMING.set_function("REQUEST_NAMED_PTFX_ASSET", LUA_NATIVE_STREAMING_REQUEST_NAMED_PTFX_ASSET);
		STREAMING.set_function("HAS_NAMED_PTFX_ASSET_LOADED", LUA_NATIVE_STREAMING_HAS_NAMED_PTFX_ASSET_LOADED);
		STREAMING.set_function("REMOVE_NAMED_PTFX_ASSET", LUA_NATIVE_STREAMING_REMOVE_NAMED_PTFX_ASSET);
		STREAMING.set_function("SET_VEHICLE_POPULATION_BUDGET", LUA_NATIVE_STREAMING_SET_VEHICLE_POPULATION_BUDGET);
		STREAMING.set_function("SET_PED_POPULATION_BUDGET", LUA_NATIVE_STREAMING_SET_PED_POPULATION_BUDGET);
		STREAMING.set_function("CLEAR_FOCUS", LUA_NATIVE_STREAMING_CLEAR_FOCUS);
		STREAMING.set_function("SET_FOCUS_POS_AND_VEL", LUA_NATIVE_STREAMING_SET_FOCUS_POS_AND_VEL);
		STREAMING.set_function("SET_FOCUS_ENTITY", LUA_NATIVE_STREAMING_SET_FOCUS_ENTITY);
		STREAMING.set_function("IS_ENTITY_FOCUS", LUA_NATIVE_STREAMING_IS_ENTITY_FOCUS);
		STREAMING.set_function("SET_RESTORE_FOCUS_ENTITY", LUA_NATIVE_STREAMING_SET_RESTORE_FOCUS_ENTITY);
		STREAMING.set_function("SET_MAPDATACULLBOX_ENABLED", LUA_NATIVE_STREAMING_SET_MAPDATACULLBOX_ENABLED);
		STREAMING.set_function("SET_ALL_MAPDATA_CULLED", LUA_NATIVE_STREAMING_SET_ALL_MAPDATA_CULLED);
		STREAMING.set_function("STREAMVOL_CREATE_SPHERE", LUA_NATIVE_STREAMING_STREAMVOL_CREATE_SPHERE);
		STREAMING.set_function("STREAMVOL_CREATE_FRUSTUM", LUA_NATIVE_STREAMING_STREAMVOL_CREATE_FRUSTUM);
		STREAMING.set_function("STREAMVOL_CREATE_LINE", LUA_NATIVE_STREAMING_STREAMVOL_CREATE_LINE);
		STREAMING.set_function("STREAMVOL_DELETE", LUA_NATIVE_STREAMING_STREAMVOL_DELETE);
		STREAMING.set_function("STREAMVOL_HAS_LOADED", LUA_NATIVE_STREAMING_STREAMVOL_HAS_LOADED);
		STREAMING.set_function("STREAMVOL_IS_VALID", LUA_NATIVE_STREAMING_STREAMVOL_IS_VALID);
		STREAMING.set_function("IS_STREAMVOL_ACTIVE", LUA_NATIVE_STREAMING_IS_STREAMVOL_ACTIVE);
		STREAMING.set_function("NEW_LOAD_SCENE_START", LUA_NATIVE_STREAMING_NEW_LOAD_SCENE_START);
		STREAMING.set_function("NEW_LOAD_SCENE_START_SPHERE", LUA_NATIVE_STREAMING_NEW_LOAD_SCENE_START_SPHERE);
		STREAMING.set_function("NEW_LOAD_SCENE_STOP", LUA_NATIVE_STREAMING_NEW_LOAD_SCENE_STOP);
		STREAMING.set_function("IS_NEW_LOAD_SCENE_ACTIVE", LUA_NATIVE_STREAMING_IS_NEW_LOAD_SCENE_ACTIVE);
		STREAMING.set_function("IS_NEW_LOAD_SCENE_LOADED", LUA_NATIVE_STREAMING_IS_NEW_LOAD_SCENE_LOADED);
		STREAMING.set_function("IS_SAFE_TO_START_PLAYER_SWITCH", LUA_NATIVE_STREAMING_IS_SAFE_TO_START_PLAYER_SWITCH);
		STREAMING.set_function("START_PLAYER_SWITCH", LUA_NATIVE_STREAMING_START_PLAYER_SWITCH);
		STREAMING.set_function("STOP_PLAYER_SWITCH", LUA_NATIVE_STREAMING_STOP_PLAYER_SWITCH);
		STREAMING.set_function("IS_PLAYER_SWITCH_IN_PROGRESS", LUA_NATIVE_STREAMING_IS_PLAYER_SWITCH_IN_PROGRESS);
		STREAMING.set_function("GET_PLAYER_SWITCH_TYPE", LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_TYPE);
		STREAMING.set_function("GET_IDEAL_PLAYER_SWITCH_TYPE", LUA_NATIVE_STREAMING_GET_IDEAL_PLAYER_SWITCH_TYPE);
		STREAMING.set_function("GET_PLAYER_SWITCH_STATE", LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_STATE);
		STREAMING.set_function("GET_PLAYER_SHORT_SWITCH_STATE", LUA_NATIVE_STREAMING_GET_PLAYER_SHORT_SWITCH_STATE);
		STREAMING.set_function("SET_PLAYER_SHORT_SWITCH_STYLE", LUA_NATIVE_STREAMING_SET_PLAYER_SHORT_SWITCH_STYLE);
		STREAMING.set_function("GET_PLAYER_SWITCH_JUMP_CUT_INDEX", LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_JUMP_CUT_INDEX);
		STREAMING.set_function("SET_PLAYER_SWITCH_OUTRO", LUA_NATIVE_STREAMING_SET_PLAYER_SWITCH_OUTRO);
		STREAMING.set_function("SET_PLAYER_SWITCH_ESTABLISHING_SHOT", LUA_NATIVE_STREAMING_SET_PLAYER_SWITCH_ESTABLISHING_SHOT);
		STREAMING.set_function("ALLOW_PLAYER_SWITCH_PAN", LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_PAN);
		STREAMING.set_function("ALLOW_PLAYER_SWITCH_OUTRO", LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_OUTRO);
		STREAMING.set_function("ALLOW_PLAYER_SWITCH_ASCENT", LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_ASCENT);
		STREAMING.set_function("ALLOW_PLAYER_SWITCH_DESCENT", LUA_NATIVE_STREAMING_ALLOW_PLAYER_SWITCH_DESCENT);
		STREAMING.set_function("IS_SWITCH_READY_FOR_DESCENT", LUA_NATIVE_STREAMING_IS_SWITCH_READY_FOR_DESCENT);
		STREAMING.set_function("ENABLE_SWITCH_PAUSE_BEFORE_DESCENT", LUA_NATIVE_STREAMING_ENABLE_SWITCH_PAUSE_BEFORE_DESCENT);
		STREAMING.set_function("DISABLE_SWITCH_OUTRO_FX", LUA_NATIVE_STREAMING_DISABLE_SWITCH_OUTRO_FX);
		STREAMING.set_function("SWITCH_TO_MULTI_FIRSTPART", LUA_NATIVE_STREAMING_SWITCH_TO_MULTI_FIRSTPART);
		STREAMING.set_function("SWITCH_TO_MULTI_SECONDPART", LUA_NATIVE_STREAMING_SWITCH_TO_MULTI_SECONDPART);
		STREAMING.set_function("IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED", LUA_NATIVE_STREAMING_IS_SWITCH_TO_MULTI_FIRSTPART_FINISHED);
		STREAMING.set_function("GET_PLAYER_SWITCH_INTERP_OUT_DURATION", LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_INTERP_OUT_DURATION);
		STREAMING.set_function("GET_PLAYER_SWITCH_INTERP_OUT_CURRENT_TIME", LUA_NATIVE_STREAMING_GET_PLAYER_SWITCH_INTERP_OUT_CURRENT_TIME);
		STREAMING.set_function("IS_SWITCH_SKIPPING_DESCENT", LUA_NATIVE_STREAMING_IS_SWITCH_SKIPPING_DESCENT);
		STREAMING.set_function("SET_SCENE_STREAMING_TRACKS_CAM_POS_THIS_FRAME", LUA_NATIVE_STREAMING_SET_SCENE_STREAMING_TRACKS_CAM_POS_THIS_FRAME);
		STREAMING.set_function("GET_LODSCALE", LUA_NATIVE_STREAMING_GET_LODSCALE);
		STREAMING.set_function("OVERRIDE_LODSCALE_THIS_FRAME", LUA_NATIVE_STREAMING_OVERRIDE_LODSCALE_THIS_FRAME);
		STREAMING.set_function("REMAP_LODSCALE_RANGE_THIS_FRAME", LUA_NATIVE_STREAMING_REMAP_LODSCALE_RANGE_THIS_FRAME);
		STREAMING.set_function("SUPPRESS_HD_MAP_STREAMING_THIS_FRAME", LUA_NATIVE_STREAMING_SUPPRESS_HD_MAP_STREAMING_THIS_FRAME);
		STREAMING.set_function("SET_RENDER_HD_ONLY", LUA_NATIVE_STREAMING_SET_RENDER_HD_ONLY);
		STREAMING.set_function("FORCE_ALLOW_TIME_BASED_FADING_THIS_FRAME", LUA_NATIVE_STREAMING_FORCE_ALLOW_TIME_BASED_FADING_THIS_FRAME);
		STREAMING.set_function("IPL_GROUP_SWAP_START", LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_START);
		STREAMING.set_function("IPL_GROUP_SWAP_CANCEL", LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_CANCEL);
		STREAMING.set_function("IPL_GROUP_SWAP_IS_READY", LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_IS_READY);
		STREAMING.set_function("IPL_GROUP_SWAP_FINISH", LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_FINISH);
		STREAMING.set_function("IPL_GROUP_SWAP_IS_ACTIVE", LUA_NATIVE_STREAMING_IPL_GROUP_SWAP_IS_ACTIVE);
		STREAMING.set_function("PREFETCH_SRL", LUA_NATIVE_STREAMING_PREFETCH_SRL);
		STREAMING.set_function("IS_SRL_LOADED", LUA_NATIVE_STREAMING_IS_SRL_LOADED);
		STREAMING.set_function("BEGIN_SRL", LUA_NATIVE_STREAMING_BEGIN_SRL);
		STREAMING.set_function("END_SRL", LUA_NATIVE_STREAMING_END_SRL);
		STREAMING.set_function("SET_SRL_TIME", LUA_NATIVE_STREAMING_SET_SRL_TIME);
		STREAMING.set_function("SET_SRL_POST_CUTSCENE_CAMERA", LUA_NATIVE_STREAMING_SET_SRL_POST_CUTSCENE_CAMERA);
		STREAMING.set_function("SET_SRL_READAHEAD_TIMES", LUA_NATIVE_STREAMING_SET_SRL_READAHEAD_TIMES);
		STREAMING.set_function("SET_SRL_LONG_JUMP_MODE", LUA_NATIVE_STREAMING_SET_SRL_LONG_JUMP_MODE);
		STREAMING.set_function("SET_SRL_FORCE_PRESTREAM", LUA_NATIVE_STREAMING_SET_SRL_FORCE_PRESTREAM);
		STREAMING.set_function("SET_HD_AREA", LUA_NATIVE_STREAMING_SET_HD_AREA);
		STREAMING.set_function("CLEAR_HD_AREA", LUA_NATIVE_STREAMING_CLEAR_HD_AREA);
		STREAMING.set_function("INIT_CREATOR_BUDGET", LUA_NATIVE_STREAMING_INIT_CREATOR_BUDGET);
		STREAMING.set_function("SHUTDOWN_CREATOR_BUDGET", LUA_NATIVE_STREAMING_SHUTDOWN_CREATOR_BUDGET);
		STREAMING.set_function("ADD_MODEL_TO_CREATOR_BUDGET", LUA_NATIVE_STREAMING_ADD_MODEL_TO_CREATOR_BUDGET);
		STREAMING.set_function("REMOVE_MODEL_FROM_CREATOR_BUDGET", LUA_NATIVE_STREAMING_REMOVE_MODEL_FROM_CREATOR_BUDGET);
		STREAMING.set_function("GET_USED_CREATOR_BUDGET", LUA_NATIVE_STREAMING_GET_USED_CREATOR_BUDGET);
		STREAMING.set_function("SET_ISLAND_ENABLED", LUA_NATIVE_STREAMING_SET_ISLAND_ENABLED);
	}
}

#endif // ENABLE_LUA