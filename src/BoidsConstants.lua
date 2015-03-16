Boids = Boids or {}
Boids.UnitFace = {
    front = 0,
    back = 1,
}

Boids.ResourceType = {
    characterFront = 0,
    characterBack = 1,
    characterDoubleFace = 2,
    mapVision = 3,
    ui = 4,
    spineEffect = 5,
}

Boids.BattleStage = {
    before = 0,
    ongoing = 1,
    pause = 2,
    win = 3,
    lose = 4,
}

Boids.UnitMovement = {
    unknown = 0,
    idle = 1,
    walking = 2,
    attacking = 3,
    casting = 4,
    casting_cont_1 = 5,
    casting_cont_2 = 6,
    casting2 = 7,
    casting2_cont_1 = 8,
    casting2_cont_2 = 9,
    die = 10,
    dead = 11,
    stunned = 12,
    appearing = 13,
}

Boids.AIMoveType = { --这个要和c++里面能对得上
    be_pushed = -1,
    stay = 0,
    initiative = 1,
    stuck = 2,
}

Boids.BuildConfig = {
    debug = 0,
    release = 1,
}

Boids.CameraMode = {
    Free = 0,
    Lock = 1,
    Follow = 2,
}

Boids.TaskState = {
    Unstart = "task_unstart",
    Start = "task_start",
    Finish = "task_finish",
    Failed = "task_failed",
}

Boids.GameState = {
    Unstart = "game_unstart",
    Start = "game_start",
    Lose = "game_lose",
    Win = "game_win",
}

Boids.UnitSource = {
    Name = "name_source",
    Tag = "tag_source",
    Type = "type_source",
    Triggered = "trigger_source"
}

Boids.UnitState = {
    Appear = "unit_appear",
    HoldAppear = "unit_hold_appear",
    MoveTo = "unit_move",
    PatrolTo = "unit_patrol",
    Patrol = "unit_patrol",
    Disappear = "unit_disappear",
    Idle = "unit_idle",
    Die = "unit_die",
    Dead = "unit_dead",
    Stunned = "unit_stunned",
    Pierce = "unit_pierce",
    Cast = "unit_cast",
    Attack = "unit_attack",
}

Boids.TriggerType = {
    MapInit = "map_init",
    UnitChange = "unit_change",
    EventChange = "event_change",
    GameChange = "game_change",
    VisionChange = "vision_change",
    ConversationChange = "conversation_change",
    Custom = "custom",
}

Boids.ActionType = {
    UnitChange = "unit_change",
    TaskChange = "task_change",
    GameChange = "game_change",
    EventChange = "event_change",
    VisionChange = "vision_change",
    WaveAction = "waves_appear",
    ConversationAction = "conversation_appear",
    Custom = "custom",
}

Boids.EventState = {
    UnStart = "event_unstart",
    Trigger = "event_trigger",
    Finish = "event_finish"
}

Boids.KnownLayer = {
    battleLayer = 10001,
    mapLayer = 10002,
    effectLayer = 10003,
    skillUILayer = 20000,
}

Boids.UnitType = {
    Enemy = "enemy",
    Neutral = "neutral",
    Ally = "ally",
    Player = "player",
    Wild = "wild",
    Opponent = "opponent"
}

Boids.Camp = Boids.UnitType

Boids.NetworkOperationType = {
    Move = 3,
    UseSkill = 4
}

Boids.CommonAnimation = {
    UnitBleed = "unit_bleed",
}

Boids.UnitBuff = {
    Normal = "normal",
    InCage = "in_cage",
    WatchNight = "watch_night",
}

Boids.FrameStage = {
    Begin = 0,
    BeginAI = 1,
    Behavior = 2,
    PostBehavior = 3,
    PostAI = 4,
    PostUpdate = 5,
}

Boids.hesitateSeconds = 0.8
Boids.wanderSpeedScale = 0.65
Boids.frameRate = 30 --这行现在不能注释掉了，里面用到了

Boids.config = Boids.BuildConfig.debug
--Boids.debugCollide = true --显示人物的碰撞半径
--Boids.debugPush = true --被推的人物显示一下
--Boids.debugGuard = true --显示人物的警戒范围
--Boids.debugAttackRange = false --显示人物的攻击范围
--Boids.showId = true --在人物上面显示个id
--Boids.extraUnitInfo = true
--Boids.hideUnit = true --不显示人物
--Boids.debugPosition = false --显示编辑器里画的位置
Boids.inEditMode = true
Boids.isCheating = false
Boids.printTraceStatisticsInterval = Boids.frameRate

--Boids.playReplay = "split.rep" --开这个则播放录像
--Boids.saveReplay = true --开这个则保存录像。可以把旧录像录成新录象
Boids.checkSnapshot = true --注释这个则忽视录像中的snapshot，避免哗哗哗刷
--Boids.saveSnapshotInterval = 1 --Boids.frameRate * 5

Boids.serverIP = "192.168.1.16"
Boids.serverPort = 6677

Boids.AI = {
    BUILDING_PRIORITY = 10000, --建筑的优先级
    UNPUSHABLE_PRIORITY = 5000, --unpushable单位的优先级
    
    MINIMAL_SPLIT_DISTANCE = 80, --如果要分成两个组，两组间最近的单位也要大于这个距离，否则就不会分成两个组
    WILL_CATCHUP_BY_PATH = 250, --落后组成员如果距离领先组的中心超过这个距离，就发动catchup。
    STOP_CATCHUP_DISTANCE = 200, --发动catchup之后追到这个距离以内就停止。如果摇杆不松，会每帧寻路

    MINIMAL_SPLIT_DISTANCE_WHEN_IN_NARROW = 70, --该参数仅在窄路时有效
    WILL_CATCHUP_BY_PATH_WHEN_IN_NARROW = 110, --该参数仅在窄路时有效
    STOP_CATCHUP_DISTANCE_WHEN_IN_NARROW = 40, --该参数仅在窄路时有效

    CATCHUP_LEVEL1_DIST = 150,
    CATCHUP_LEVEL1_SPEED_SCALE = 1.0,

    CATCHUP_LEVEL2_DIST = 550,
    CATCHUP_LEVEL2_SPEED_SCALE = 1.7,

    CATCHUP_LEVEL3_DIST = 1000,
    CATCHUP_LEVEL3_SPEED_SCALE = 2,
    
    PATH_VALIDITY = 1000000,

    ENABLE_PUSH = true,

    --PAINT_PATH = true, --开这个的话寻路的路径会画出来
    --PAINT_MESH = 30, --开这个的话会显示多边形
    --PAINT_STEP = true, --开这个的话每一帧的位移(原始意愿，不考虑障碍物的)会画出来
    --TICK = 20, --开这个的话每tick帧会出个log
    --NOT_LOAD_NAV = true --开这个的话不会使用预处理好的.nav文件，重新生成
}
