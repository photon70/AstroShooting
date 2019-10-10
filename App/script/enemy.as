#include "fomalhout.as"
#include "pegasus.as"
#include "algedi.as"
#include "delphinus.as"
#include "waterjar.as"
#include "m15.as"
#include "mira.as"
#include "algol.as"
#include "mesarthim.as"
#include "capella.as"
#include "m31.as"

class EBTemplate : EnemyBehavior{
    Texture tex("AS/star_1.png");

    void Update(GameInterface &inout inter, EnemyData &inout data){
        
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};