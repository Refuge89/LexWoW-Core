/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2008 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"

initialiseSingleton( ChannelMgr );

void WorldSession::HandleChannelJoin(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	uint32 dbc_id = 0;
	uint16 crap;		// crap = some sort of channel type? Actually it seems that this is 2 bytes and not 1 uint16
	Channel * chn;

	recvPacket >> crap;	// seems like 00 and 01 
	recvPacket >> dbc_id;
	recvPacket >> channelname;
	recvPacket >> pass;

	if(!stricmp(channelname.c_str(), "LookingForGroup") && !sWorld.m_lfgForNonLfg)
	{
/*		// make sure we have lfg dungeons
		for(uint32 i = 0; i < MAX_LFG_QUEUE_ID; ++i)
		{
			if(_player->LfgDungeonId[i] != 0)
				break;
		}

		if(i == MAX_LFG_QUEUE_ID)
			return;		// don't join lfg
			*/

		//are we looking to any dungeons at all ?
		if( _player->LFG_dungeons.begin() == NULL )
			return;
	}

	if( sWorld.GmClientChannel.size() && !stricmp(sWorld.GmClientChannel.c_str(), channelname.c_str()) && !GetPermissionCount())
	{ 
		return;
	}
	
	chn = channelmgr.GetCreateChannel(channelname.c_str(), _player, dbc_id);
	if(chn == NULL)
	{ 
		return;
	}

	chn->AttemptJoin(_player, pass.c_str());
	Log.Debug("ChannelJoin", "%s", channelname.c_str());
}

void WorldSession::HandleChannelLeave(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	uint32 code = 0;
	Channel * chn;

	recvPacket >> code;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn == NULL)
	{ 
		return;
	}

	chn->Part(_player);
}

void WorldSession::HandleChannelList(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;

	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn != NULL)
		chn->List(_player);
}

void WorldSession::HandleChannelPassword(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> pass;
	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if(chn)
		chn->Password(_player, pass.c_str());
}

void WorldSession::HandleChannelSetOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->SetOwner(_player, plr);
}

void WorldSession::HandleChannelOwner(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,pass;
	Channel * chn;

	recvPacket >> channelname;
	chn = channelmgr.GetChannel(channelname.c_str(),_player);
	if(chn)
		chn->GetOwner(_player);
}

void WorldSession::HandleChannelModerator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->GiveModerator(_player, plr);
}

void WorldSession::HandleChannelUnmoderator(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->TakeModerator(_player, plr);
}

void WorldSession::HandleChannelMute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Mute(_player, plr);
}

void WorldSession::HandleChannelUnmute(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Unmute(_player, plr);
}

void WorldSession::HandleChannelInvite(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Invite(_player, plr);
}
void WorldSession::HandleChannelKick(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Kick(_player, plr, false);
}

void WorldSession::HandleChannelBan(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	Player * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayer(newp.c_str(), false);
	if(chn && plr)
		chn->Kick(_player, plr, true);
}

void WorldSession::HandleChannelUnban(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname,newp;
	Channel * chn;
	PlayerInfo * plr;

	recvPacket >> newp;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	plr = objmgr.GetPlayerInfoByName(newp.c_str());
	if(chn && plr)
		chn->Unban(_player, plr);
}

void WorldSession::HandleChannelAnnounce(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;
	
	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn)
		chn->Announce(_player);
}

void WorldSession::HandleChannelModerate(WorldPacket& recvPacket)
{
	CHECK_PACKET_SIZE(recvPacket, 1);
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn)
		chn->Moderate(_player);
}

void WorldSession::HandleChannelRosterQuery(WorldPacket & recvPacket)
{
	string channelname;
	Channel * chn;
	recvPacket >> channelname;

	chn = channelmgr.GetChannel(channelname.c_str(), _player);
	if(chn)
		chn->List(_player);
}

void WorldSession::HandleChannelNumMembersQuery(WorldPacket & recvPacket)
{
/*	string channel_name;
	WorldPacket data(SMSG_CHANNEL_NUM_MEMBERS_QUERY_RESPONSE, recvPacket.size() + 4);
	Channel *chn;
	recvPacket >> channel_name;
	chn = channelmgr.GetChannel(channel_name.c_str(), _player);
	if(chn)
	{
		data << channel_name;
		data << uint8(chn->m_flags);
		data << uint32(chn->GetNumMembers());
		SendPacket(&data);
	}*/
}