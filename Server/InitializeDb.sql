DROP TABLE IF EXISTS accounts;
DROP TABLE IF EXISTS players;
DROP TABLE IF EXISTS npcs;
DROP TABLE IF EXISTS items;
CREATE TABLE accounts (
    id integer not null primary key autoincrement,
    valid integer not null,
    accountname text not null unique collate nocase,
    email text not null collate nocase,
    session text collate nocase,
    playing integer not null,
    key text not null collate nocase,
    salt text not null collate nocase
);
CREATE TABLE players (
    -- Identification:
    id integer not null primary key autoincrement,
    accountid numeric not null,
    firstname text not null,
    lastname text not null,
    guild text,
    race text not null,
    gender text not null,
    face text not null,
    skin text not null,
    profession text not null,
    level integer not null,
    -- Stats:
    hp integer not null,
    maxhp integer not null,
    bp integer not null,
    maxbp integer not null,
    mp integer not null,
    maxmp integer not null,
    ep integer not null,
    maxep integer not null,
    strength integer not null,
    constitution integer not null,
    intelligence integer not null,
    dexterity integer not null,
    -- Skills:
    axe integer not null,
    dagger integer not null,
    unarmed integer not null,
    hammer integer not null,
    polearm integer not null,
    spear integer not null,
    staff integer not null,
    sword integer not null,
    archery integer not null,
    crossbow integer not null,
    sling integer not null,
    thrown integer not null,
    armor integer not null,
    dualWeapon integer not null,
    shield integer not null,
    bardic integer not null,
    conjuring integer not null,
    druidic integer not null,
    illusion integer not null,
    necromancy integer not null,
    sorcery integer not null,
    shamanic integer not null,
    spellcraft integer not null,
    summoning integer not null,
    focus integer not null,
    armorsmithing integer not null,
    tailoring integer not null,
    fletching integer not null,
    weaponsmithing integer not null,
    alchemy integer not null,
    lapidary integer not null,
    calligraphy integer not null,
    enchanting integer not null,
    herbalism integer not null,
    hunting integer not null,
    mining integer not null,
    bargaining integer not null,
    camping integer not null,
    firstaid integer not null,
    lore integer not null,
    pickLocks integer not null,
    scouting integer not null,
    search integer not null,
    stealth integer not null,
    traps integer not null,
    aeolandis integer not null,
    hieroform integer not null,
    highgundis integer not null,
    oldpraxic integer not null,
    praxic integer not null,
    runic integer not null,
    -- Equipment:
    head text not null,
    chest text not null,
    arms text not null,
    hands text not null,
    legs text not null,
    feet text not null,
    cloak text not null,
    necklace text not null,
    ringone text not null,
    ringtwo text not null,
    righthand text not null,
    lefthand text not null,
    -- Location:
    zone text not null,
    x real not null,
    y real not null,
    z real not null,
    pitch real not null,
    yaw real not null
);
CREATE TABLE npcs (
    -- Identification:
    id integer not null primary key autoincrement,
    firstname text not null,
    lastname text not null,
    guild text,
    race text not null,
    gender text not null,
    face text not null,
    skin text not null,
    level integer not null,
    hp integer not null,
    maxhp integer not null,
    bp integer not null,
    maxbp integer not null,
    mp integer not null,
    maxmp integer not null,
    ep integer not null,
    maxep integer not null,
    strength integer not null,
    constitution integer not null,
    intelligence integer not null,
    dexterity integer not null,
    -- Equipment:
    head text not null,
    chest text not null,
    arms text not null,
    hands text not null,
    legs text not null,
    feet text not null,
    cloak text not null,
    necklace text not null,
    ringone text not null,
    ringtwo text not null,
    righthand text not null,
    lefthand text not null,
    -- Location:
    zone text not null,
    x real not null,
    y real not null,
    z real not null,
    pitch real not null,
    yaw real not null
);
CREATE TABLE items (
    id integer not null primary key autoincrement,
    name text not null,
    location text not null,
    weight real not null,
    equip integer not null,
    use integer not null,
    x integer not null,
    y integer not null
);
