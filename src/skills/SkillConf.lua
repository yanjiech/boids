SkillConf = SkillConf or {}

SkillConf.skill = {
    BarrageOfArrow = {
        hint = "arrow",
        damage = { 50.0, 100.0, 200.0 },
        cd = 8.0,
        interval = 0.33,
        speed = 1000.0,
        range = 500.0,
        angle = 30.0,
        radius = 50.0,
        count_per_wave = 5,
        waves= 5
    },
    BatSwarm = {
        hint = "circle",
        count = 5,
        duration = 1.5,
        damage = 50.0,
        range = 200.0,
        radius = 70.0,
        cd = 6.0
    },
    BonesRain = {
        hint = "arrow",
        damage = 50.0,
        cd = 10.0,
        radius = 50.0,
        count = 5,
        interval = 0.1,
        range = 0
    },
    BonesSacrifice = {
        hint = "circle",
        cd = 14.0,
        radius = 60.0,
        distance = 300.0,
        range = 0
    },
    Boomerang = {
        hint = "circle",
        damage = 30.0,
        cd = 15.0,
        speed = 400.0,
        maxstep = 5,
        maxgapsq = 57600.0,
        range = 240.0
    },
    CrazyShoot = {
        hint = "circle",
        count = 5,
        speed = 800.0,
        damage = 35.0,
        interval = 0.1,
        range = 400.0,
        cd = 5.0
    },
    DarkPierce = {
        hint = "circle",
        damage = 50.0,
        cd = 5.0,
        range = 300.0,
        stun = 2.0,
        count = 5
    },
    DeathField = {
        hint = "arrow",
        damage = 40,
        duration = 5.0,
        cd = 15.0,
        radius = 150.0,
        range = 240.0
    },
    DeathTornado = {
        hint = "arrow",
        damage = 100.0,
        cd = 12.0,
        radius = 100,
        speed = 500.0,
        delay = 0.8,
        duration = 2.8,
        range = 0
    },
    EagleAttack = {
        hint = "arrow",
        damage = 50.0,
        cd = 12.0,
        neglp = 0.3,
        speed = 1500,
        radius = 100.0,
        range = 320.0
    },
    EvilInvoke = {
        hint = "circle",
        count = 2,
        radius = 100.0,
        cd = 10.0,
        duration = 1.3,
        range = 0
    },
    GiantSword = {
        hint = "dyn_circle",
        damage = 50.0,
        cd = 4.0,
        radius = 150.0,
        range = 150.0,
        min_range = 150.0,
        max_range = 300.0,
        stun = 2.0
    },
    Impetus = {
        damage = { 50.0, 100.0, 200.0 },
        cd = 6.0,
        buff_duration = 3.0,
        range = 500.0
    },
    LightningChain = {
        hint = "circle",
        duration = 0.5,
        cd = 6.0,
        damage = { 50.0, 100.0, 150.0 },
        length = 265.0,
        range = 400.0,
        count = 5,
        interval = 0.1
    },
    LightShield = {
        hint = "none",
        cd = 12.0,
        hp = { 100.0, 200.0, 300.0 },
        duration = 6.0
    },
    MagicPulse = {
        hint = "circle",
        damage = { 10.0, 20.0, 30.0 },
        range = 400.0,
        duration = 5.0,
        interval = 0.5,
        length = 260.0,
        cd = 6.0
    },
    MissileShowers = {
        hint = "circle",
        damage = { 50.0, 100.0 },
        cd = 6.0,
        range = 400.0,
        count = 3,
        interval = 0.3
    },
    NaturesAttendants = {
        hint = "circle",
        heal = { 20.0, 40.0, 60.0 },
        cd = 15.0,
        count = { 4, 6, 8, 10 },
        range = 500.0,
        interval = 1.0,
        init_speed = 300.0,
        accel = 600.0,
        duration = 10.0
    },
    PandaFire = {
        hint = "arrow",
        damage = 50.0,
        radius = 60.0,
        cd = 5.0,
        count = 6,
        buff_duration = 2.0,
        interval = 0.08
    },
    SwordWave = {
        hint = "arrow",
        damage = 50.0,
        cd = 11.0,
        radius = 100.0,
        affect_range = 500.0,
        range = 0
    },
    WrathOfThunder = {
        hint = "circle",
        count = 5,
        cd = 6.0,
        damage = 50.0,
        range = 500.0
    }
}

