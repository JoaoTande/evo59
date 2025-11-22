#include "config.h"
#include <vector>

#define STRINGIFY(x) #x
#define STRINGIFY2(x) STRINGIFY(x)

typedef struct CONFIG_FIELD {
    char sector[127];
    char key[127];
    char value[127];

}CONFIG_FIELD;

#define MAX_DEFAULT_CONFIG 6


CONFIG_FIELD default_config[MAX_DEFAULT_CONFIG] = {
    {"game", "fullscreen", "0"},
    {"game", "zoom", "1"},
    {"game", "speed", "1"},
    {"","",0},
    {"","",0},
    {"","",0}


};


Config::Config() :
    cfg(nullptr)
{

}

bool Config::loadFile(const std::string filepath){
    ALLEGRO_PATH *path = nullptr;

    path = al_create_path(filepath.c_str());
    this->cfg = al_load_config_file(al_path_cstr(path,ALLEGRO_NATIVE_DRIVE_SEP));

    if(!this->cfg){
        //se nao encontrar gerar um novo
        this->cfg = createDefaultConfig();
        if(!this->cfg){
            return false;
        }

        saveFile("config.cfg");
        return true;
    }

    al_destroy_path(path);
    return true;

}

bool Config::saveFile(const std::string filepath){

    if(!cfg){
        return false;
    }
    ALLEGRO_PATH *path = nullptr;
    path = al_create_path(filepath.c_str());
    al_save_config_file(al_path_cstr(path,ALLEGRO_NATIVE_DRIVE_SEP), cfg);
    al_destroy_path(path);
    return true;
}

ALLEGRO_CONFIG* Config::createDefaultConfig(){
    ALLEGRO_PATH *path = nullptr;
    ALLEGRO_CONFIG *tmp_cfg = nullptr;

    path = al_create_path("config.cfg");
    tmp_cfg = al_create_config();
    al_add_config_section(tmp_cfg, "game");


    for(int i = 0; i < MAX_DEFAULT_CONFIG; i++){
        CONFIG_FIELD *f = &default_config[i];
        if(strlen(f->key) == 0 || strlen(f->sector) == 0) continue;
        al_set_config_value(tmp_cfg, f->sector, f->key, f->value);
    }

    al_destroy_path(path);

    return tmp_cfg;
}



bool  Config::getConfigBool(Config& cfg, const char *sector, const char *key){
    bool result = false;
    const char *val = al_get_config_value(cfg.cfg, sector, key);

    if(strcmp(val, "true") == 0){
        result = true;
    }

    return result;

}

int   Config::getConfigInt(Config& cfg, const char *sector, const char *key){
    const char *val = al_get_config_value(cfg.cfg, sector, key);
    return std::atoi(val);
}
float Config::getConfigFloat(Config& cfg, const char *sector, const char *key){
    const char *val = al_get_config_value(cfg.cfg, sector, key);
    return std::atof(val);
}

void Config::Unload(){
    if(cfg){
        al_destroy_config(cfg);
    }
}

