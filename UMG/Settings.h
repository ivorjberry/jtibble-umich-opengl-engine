// To disable shaders, comment the following line:
// Also enables the check in main.cpp to see if shaders are allowed
#define ENABLE_SHADERS

////////////////////////////
//To get weapon stats, mount points, etc
//WEAPON MOUNT POINTS
#define MINIGUN_MOUNT_POSITION_X 0.0f;
#define MINIGUN_MOUNT_POSITION_Y 0.5f;
#define MINIGUN_MOUNT_POSITION_Z 0.0f;

#define MINE_MOUNT_POSITION_X 0.0f;
#define MINE_MOUNT_POSITION_Y 0.0f;
#define MINE_MOUNT_POSITION_Z 0.0f;

#define MISSILE_MOUNT_POSITION_X 0.0f;
#define MISSILE_MOUNT_POSITION_Y 1.0f;
#define MISSILE_MOUNT_POSITION_Z 0.0f;

#define BOMB_MOUNT_POSITION_X 0.0f;
#define BOMB_MOUNT_POSITION_Y 1.0f;
#define BOMB_MOUNT_POSITION_Z 0.0f;

#define SMISSILE_MOUNT_POSITION_X 0.0f;
#define SMISSILE_MOUNT_POSITION_Y 0.5f;
#define SMISSILE_MOUNT_POSITION_Z 0.0f;

#define PMISSILE_MOUNT_POSITION_X 0.0f;
#define PMISSILE_MOUNT_POSITION_Y 0.5f;
#define PMISSILE_MOUNT_POSITION_Z 0.0f;

//WEAPON MOUNT ORIENTATION
#define MINIGUN_MOUNT_ORIENTATION_ROLL 0.0f;
#define MINIGUN_MOUNT_ORIENTATION_PITCH 0.0f;
#define MINIGUN_MOUNT_ORIENTATION_YAW 90.0f;

#define MINE_MOUNT_ORIENTATION_ROLL 0.0f;
#define MINE_MOUNT_ORIENTATION_PITCH 0.0f;
#define MINE_MOUNT_ORIENTATION_YAW 0.0f;

#define BOMB_MOUNT_ORIENTATION_ROLL 0.0f;
#define BOMB_MOUNT_ORIENTATION_PITCH 0.0f;
#define BOMB_MOUNT_ORIENTATION_YAW 0.0f;

#define MISSILE_MOUNT_ORIENTATION_ROLL 0.0f;
#define MISSILE_MOUNT_ORIENTATION_PITCH 90.0f;
#define MISSILE_MOUNT_ORIENTATION_YAW 0.0f;

#define SMISSILE_MOUNT_ORIENTATION_ROLL 0.0f;
#define SMISSILE_MOUNT_ORIENTATION_PITCH 0.0f;
#define SMISSILE_MOUNT_ORIENTATION_YAW 90.0f;

#define PMISSILE_MOUNT_ORIENTATION_ROLL 0.0f;
#define PMISSILE_MOUNT_ORIENTATION_PITCH 0.0f;
#define PMISSILE_MOUNT_ORIENTATION_YAW 90.0f;

//WEAPON MOUNT LENGTH
#define MINIGUN_MOUNT_LENGTH 11.0f;
#define MINE_MOUNT_LENGTH 0.0f;
#define MISSILE_MOUNT_LENGTH 0.0f;
#define BOMB_MOUNT_LENGTH 0.0f;
#define SMISSILE_MOUNT_LENGTH 11.0f;
#define PMISSILE_MOUNT_LENGTH 11.0f;

//WEAPON PROJECTILE SPEED
#define MINIGUN_SPEED 5.0f;
#define MINE_SPEED 0.0f;
#define MISSILE_SPEED 4.0f;
#define BOMB_SPEED 3.0f;
#define SMISSILE_SPEED 6.0f;
#define PMISSILE_SPEED 5.0f;

//WEAPON PROJECTILE DAMAGE
#define MINIGUN_DAMAGE 2.0f;
#define MINE_DAMAGE 49.0f;
#define MISSILE_DAMAGE 7.0f;
#define BOMB_DAMAGE 32.0f;
#define SMISSILE_DAMAGE 3.0f;
#define PMISSILE_DAMAGE 15.0f;

//WEAPON AMMO HELD
#define MINIGUN_MAX_ROUNDS 100;
#define MINE_MAX_ROUNDS 5;
#define BOMB_MAX_ROUNDS 3;
#define MISSILE_MAX_ROUNDS 14; //tentative on all of these
#define SMISSILE_MAX_ROUNDS 60;
#define PMISSILE_MAX_ROUNDS 8;

//WEAPON PROJECTILE COLOR
#define MINIGUN_PROJECTILE_COLOR_R 1.0f;
#define MINIGUN_PROJECTILE_COLOR_G 242/255.0f;
#define MINIGUN_PROJECTILE_COLOR_B 0.0f;
#define MINIGUN_PROJECTILE_COLOR_A 1.0f;

#define MINE_PROJECTILE_COLOR_R 0.0f;
#define MINE_PROJECTILE_COLOR_G 1.0f; 
#define MINE_PROJECTILE_COLOR_B 0.0f;
#define MINE_PROJECTILE_COLOR_A 1.0f;

#define MISSILE_PROJECTILE_COLOR_R 1.0f;
#define MISSILE_PROJECTILE_COLOR_G 0.0f;
#define MISSILE_PROJECTILE_COLOR_B 0.0f;
#define MISSILE_PROJECTILE_COLOR_A 1.0f;

#define BOMB_PROJECTILE_COLOR_R 128/255.0f;
#define BOMB_PROJECTILE_COLOR_G 0.0f;
#define BOMB_PROJECTILE_COLOR_B 64/255.0f;
#define BOMB_PROJECTILE_COLOR_A 1.0f;

#define SMISSILE_PROJECTILE_COLOR_R 1.0f;
#define SMISSILE_PROJECTILE_COLOR_G 127/255.0f;
#define SMISSILE_PROJECTILE_COLOR_B 39/255.0f;
#define SMISSILE_PROJECTILE_COLOR_A 1.0f;

#define PMISSILE_PROJECTILE_COLOR_R 0.7f;
#define PMISSILE_PROJECTILE_COLOR_G 0.7f;
#define PMISSILE_PROJECTILE_COLOR_B 0.7f;
#define PMISSILE_PROJECTILE_COLOR_A 1.0f;

//WEAPON COOLDOWN TIME
#define MINIGUN_COOLDOWN 0.1f;
#define MINE_COOLDOWN 0.5f;
#define BOMB_COOLDOWN 0.5f;
#define MISSILE_COOLDOWN 0.5f;
#define SMISSILE_COOLDOWN 0.15f;
#define PMISSILE_COOLDOWN 1.0f;

//WEAPON ACTIVATION TIME
#define MINIGUN_ACTIVATION 0.0f;
#define MISSILE_ACTIVATION 0.15f;
#define MINE_ACTIVATION 0.5f;
#define BOMB_ACTIVATION 0.25f;
#define SMISSILE_ACTIVATION 0.0f;
#define pMISSILE_ACTIVATION 0.0f;

//WEAPON LIFETIME
//#define MINIGUN_LIFETIME 99999.9f;
#define MISSILE_LIFETIME 5.0f;
#define MINE_LIFETIME 90.0f;
#define BOMB_LIFETIME 90.0f;

//AI
#define NUMBER_AI 4;
#define AI_AGGRESSION_ENABLED true; //change this to keep the AI from shooting
//#define GOD_MODE true;

//ITEM RESPAWN
#define ITEM_MIN_RESPAWN 15.0f;
#define ITEM_MAX_RESPAWN 30.0f;

