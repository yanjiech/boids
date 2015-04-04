//
//  BoidsConstant.h
//  Boids
//
//  Created by Yanjie Chen on 3/3/15.
//
//

#ifndef __Boids__BoidsConstant__
#define __Boids__BoidsConstant__

#define FLIPPED_HORIZONTALLY 0x80000000
#define FLIPPED_VERTICALLY 0x40000000
#define FLIPPED_DIAGONALLY 0x20000000

#define UNIT_BLEED "unit_bleed"

#define UNIT_SOURCE_NAME "name_source"
#define UNIT_SOURCE_TAG "tag_source"
#define UNIT_SOURCE_TYPE "type_source"
#define UNIT_SOURCE_TRIGGERED "triggered_source"

#define UNIT_STATE_APPEAR "unit_appear"
#define UNIT_STATE_HOLD_APPEAR "unit_hold_appear"
#define UNIT_STATE_MOVE_TO "unit_move"
#define UNIT_STATE_PATROL_TO "unit_patrol"
#define UNIT_STATE_PATROL "unit_patrol"
#define UNIT_STATE_DISAPPEAR "unit_disappear"
#define UNIT_STATE_IDLE "unit_idle"
#define UNIT_STATE_DIE "unit_die"
#define UNIT_STATE_DEAD "unit_dead"
#define UNIT_STATE_ALIVE "unit_alive"
#define UNIT_STATE_STUNNED "unit_stunned"
#define UNIT_STATE_PIERCE "unit_pierce"
#define UNIT_STATE_CAST "unit_cast"
#define UNIT_STATE_ATTACK "unit_attack"

#define UNIT_CAMP_ENEMY "enemy"
#define UNIT_CAMP_NEUTRAL "neutral"
#define UNIT_CAMP_ALLY "ally"
#define UNIT_CAMP_PLAYER "player"
#define UNIT_CAMP_WILD "wild"
#define UNIT_CAMP_OPPONENT "opponent"

#define PAINT_PATH 0
#define PAINT_MESH 20

#define SKILL_NAME_WRATH_OF_THUNDER "WrathOfThunder"
#define SKILL_NAME_LIGHTNING_CHAIN "LightningChain"
#define SKILL_NAME_BARRAGE_OF_ARROW "BarrageOfArrow"
#define SKILL_NAME_CRAZYSHOOT "CrazyShoot"
#define SKILL_NAME_GIANT_SWORD "GiantSword"
#define SKILL_NAME_LIGHTSHIELD "LightShield"
#define SKILL_NAME_DARK_PIERCE "DarkPierce"
#define SKILL_NAME_BAT_SWARM "BatSwarm"
#define SKILL_NAME_NATURESATTENDANTS "NaturesAttendants"
#define SKILL_NAME_IMPETUS "Impetus"
#define SKILL_NAME_BONESRAIN "BonesRain"
#define SKILL_NAME_MAGIC_PULSE "MagicPulse"
#define SKILL_NAME_MISSILE_SHOWERS "MissileShowers"

#define GAME_TASK_TYPE_LIMITED_TIME "limited_time"
#define GAME_TASK_TYPE_SURVIVE "survive"
#define GAME_TASK_TYPE_ELIMINATE_UNIT "eliminate_unit"
#define GAME_TASK_TYPE_COLLECT_ITEM "collect_item"
#define GAME_TASK_TYPE_ESCAPE "escape"

#define BUFF_TYPE_SHIELD "buff_shield"
#define BUFF_TYPE_STUN "buff_stun"
#define BUFF_TYPE_PIERCE "buff_pierce"
#define BUFF_TYPE_BURN "buff_burn"
#define BUFF_TYPE_POISON "buff_poison"

#define EVENT_TRIGGER_TYPE_MAP_INIT "map_init"
#define EVENT_TRIGGER_TYPE_UNIT_CHANGE "unit_change"
#define EVENT_TRIGGER_TYPE_UNIT_STAY "unit_stay"
#define EVENT_TRIGGER_TYPE_EVENT_CHANGE "event_change"
#define EVENT_TRIGGER_TYPE_GAME_CHANGE "game_change"
#define EVENT_TRIGGER_TYPE_VISION_CHANGE "vision_change"
#define EVENT_TRIGGER_TYPE_CONVERSATION_CHANGE "conversation_change"
#define EVENT_TRIGGER_TYPE_CUSTOM "custom"

#define EVENT_STATE_UNSTRART "event_unstart"
#define EVENT_STATE_TRIGGER "event_trigger"
#define EVENT_STATE_FINISH "event_finish"
#define EVENT_STATE_ENABLE "event_enable"

#define GAME_TASK_STATE_UNSTARTED "task_unstart"
#define GAME_TASK_STATE_STARTED "task_start"
#define GAME_TASK_STATE_FAILED "task_failed"
#define GAME_TASK_STATE_FINISHED "task_finished"

#define GAME_STATE_PREPARE "game_prepare"
#define GAME_STATE_RUNNING "game_running"
#define GAME_STATE_PAUSED "game_paused"
#define GAME_STATE_WIN "game_win"
#define GAME_STATE_LOSE "game_lose"

#define ACTION_TYPE_UNIT_CHANGE "unit_change"
#define ACTION_TYPE_TASK_CHANGE "task_change"
#define ACTION_TYPE_GAME_CHANGE "game_change"
#define ACTION_TYPE_EVENT_CHANGE "event_change"
#define ACTION_TYPE_VISION_CHANGE "vision_change"
#define ACTION_TYPE_WAVE_ACTION "wave_action"
#define ACTION_TYPE_CONVERSATION_ACTION "conversation_action"
#define ACTION_TYPE_CUSTOM "custom"

#endif /* defined(__Boids__BoidsConstant__) */
