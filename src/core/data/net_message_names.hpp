#pragma once

namespace big
{
    struct NetMessageName
	{
		uint32_t id;
		const char name[64];
	};

	const NetMessageName net_message_names[]
	{
		{ 0xFFFFF,  "MsgInvalid"},
		{ 0x62,     "MsgSessionAcceptChat"},
		{ 0x2D,     "MsgStartMatchCmd"},
		{ 0x1F,     "MsgSetInvitableCmd"},
		{ 0x23,     "MsgSessionMemberIds"},
		{ 0x54,     "MsgRequestGamerInfo"},
		{ 0x53,     "MsgRemoveGamersFromSessionCmd"},
		{ 0x35,     "MsgNotMigrating"},
		{ 0x12,     "MsgMigrateHostResponse"},
		{ 0x66,     "MsgMigrateHostRequest"},
		{ 0x2A,     "MsgJoinResponse"},
		{ 0x41,     "MsgJoinRequest"},
		{ 0x58,     "MsgHostLeftWhilstJoiningCmd"},
		{ 0x5F,     "MsgConfigResponse"},
		{ 0x48,     "MsgConfigRequest"},
		{ 0x5A,     "MsgChangeSessionAttributesCmd"},
		{ 0x64,     "MsgAddGamerToSessionCmd"}, // this is where send net info to lobby is called, among other things
		{ 0x10,     "MsgReassignResponse"},
		{ 0x01,     "MsgReassignNegotiate"},
		{ 0x26,     "MsgReassignConfirm"},
		{ 0x18,     "MsgPlayerData"},
		{ 0x30,     "MsgPackedReliables"},
		{ 0x3B,     "MsgPackedCloneSyncACKs"},
		{ 0x16,     "MsgNonPhysicalData"},
		{ 0x5D,     "MsgNetArrayMgrUpdateAck"},
		{ 0x60,     "MsgNetArrayMgrUpdate"},
		{ 0x25,     "MsgNetArrayMgrSplitUpdateAck"},
		{ 0x0B,     "MsgScriptVerifyHostAck"},
		{ 0x3E,     "MsgScriptVerifyHost"},
		{ 0x0E,     "MsgScriptNewHost"},
		{ 0x1A,     "MsgScriptMigrateHostFailAck"},
		{ 0x33,     "MsgScriptMigrateHost"},
		{ 0x40,     "MsgScriptLeaveAck"},
		{ 0x17,     "MsgScriptLeave"},
		{ 0x4D,     "MsgScriptJoinHostAck"},
		{ 0x43,     "MsgScriptJoinAck"},
		{ 0x5C,     "MsgScriptJoin"},
		{ 0x67,     "MsgScriptHostRequest"},
		{ 0x5B,     "MsgScriptHandshakeAck"},
		{ 0x57,     "MsgScriptHandshake"},
		{ 0x2B,     "MsgScriptBotLeave"}, // unused?
		{ 0x63,     "MsgScriptBotJoinAck"}, // unused?
		{ 0x1C,     "MsgScriptBotJoin"}, // unused?
		{ 0x31,     "MsgScriptBotHandshakeAck"}, // unused?
		{ 0x4B,     "MsgScriptBotHandshake"}, // unused?
		{ 0x3D,     "MsgPartyLeaveGame"},
		{ 0x1E,     "MsgPartyEnterGame"},
		{ 0x4E,     "MsgCloneSync"}, // aka clone_create, clone_sync etc.
		{ 0x65,     "MsgActivateNetworkBot"}, // unused?
		{ 0x29,     "MsgRequestObjectIds"},
		{ 0x09,     "MsgInformObjectIds"},
		{ 0x24,     "MsgTextMessage"}, // this one is for chat
		{ 0x61,     "MsgPlayerIsTyping"},
		{ 0x4F,     "MsgPackedEvents"}, // aka received_event
		{ 0x20,     "MsgPackedEventReliablesMsgs"},
		{ 0x0D,     "MsgRequestKickFromHost"},
		{ 0x50,     "MsgTransitionToGameStart"},
		{ 0x02,     "MsgTransitionToGameNotify"},
		{ 0x06,     "MsgTransitionToActivityStart"},
		{ 0x36,     "MsgTransitionToActivityFinish"},
		{ 0x3C,     "MsgTransitionParameters"},
		{ 0x37,     "MsgTransitionParameterString"},
		{ 0x1B,     "MsgTransitionLaunchNotify"},
		{ 0x19,     "MsgTransitionLaunch"},
		{ 0x14,     "MsgTransitionGamerInstruction"},
		{ 0x0A,     "MsgTextMessage2"}, // this one is for phone message
		{ 0x52,     "MsgSessionEstablishedRequest"},
		{ 0x07,     "MsgSessionEstablished"},
		{ 0x42,     "MsgRequestTransitionParameters"},
		{ 0x47,     "MsgRadioStationSyncRequest"},
		{ 0x46,     "MsgRadioStationSync"},
		{ 0x3A,     "MsgPlayerCardSync"},
		{ 0x6A,     "MsgPlayerCardRequest"},
		{ 0x81,     "MsgLostConnectionToHost"},
		{ 0x34,     "MsgKickPlayer"}, // host kick
		{ 0x7E,     "MsgDebugStall"}, // unused?
		{ 0x59,     "MsgCheckQueuedJoinRequestReply"},
		{ 0x51,     "MsgCheckQueuedJoinRequest"},
		{ 0x0C,     "MsgBlacklist"},
		{ 0x83,     "MsgRoamingRequestBubbleRequiredResponse"},
		{ 0x82,     "MsgRoamingRequestBubbleRequiredCheck"},
		{ 0x2E,     "MsgRoamingRequestBubble"},
		{ 0x4C,     "MsgRoamingJoinBubble"},
		{ 0x3F,     "MsgRoamingJoinBubbleAck"},
		{ 0x32,     "MsgRoamingInitialBubble"},
		{ 0x03,     "MsgVoiceStatus"},
		{ 0x00,     "MsgTextChatStatus"},
		{ 0x08,     "MsgJoinResponse2"},
		{ 0x68,     "MsgJoinRequest2"},
		{ 0x38,     "MsgNetTimeSync"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 37
		{ 0x55,     "MsgNetComplaint"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 54
		{ 0x27,     "MsgNetLagPing"}, // unused? ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 26
		{ 0x6B,     "MsgSearchResponse"}, // unused? ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 6A
		{ 0x05,     "MsgSearchRequest"}, // unused?
		{ 0x2C,     "MsgQosProbeResponse"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 2B
		{ 0x1D,     "MsgQosProbeRequest"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 1C
		{ 0x49,     "MsgCxnRelayAddressChanged"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 48
		{ 0x2F,     "MsgCxnRequestRemoteTimeout"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 2E
		{ 0x22,     "MsgSessionDetailRequest"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 21
		{ 0x13,     "MsgSessionDetailResponse"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 12
		{ 0x0F,     "MsgKeyExchangeOffer"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 0E (last result)
		{ 0x44,     "MsgKeyExchangeAnswer"}, // ctor 40 53 48 83 EC 20 BA ? ? ? ? 4C 8D 0D ? ? ? ? 48 8B D9 44 8D 42 43
		{ 0x87,     "Msg_0x87"},
		{ 0x88,     "Msg_0x88"},
		{ 0x80,     "Msg_0x80"},
		{ 0x28,     "Msg_0x28"},
		{ 0x11,     "Msg_0x11"},
		{ 0x45,     "Msg_0x45"},
		{ 0x89,     "Msg_0x89"},
		{ 0x86,     "Msg_0x86"},
	};
}
