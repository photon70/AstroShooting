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
#include "pleiades.as"

#include "polaris.as"
#include "cassiopeia.as"
#include "cepheiDelta.as"
#include "ETCluster.as"
#include "garnetStar.as"
#include "doubleCluster.as"

#include "summerTriangle.as"
#include "cygni61.as"
#include "m57.as"
#include "m13.as"
#include "lyraeEpsilon.as"
#include "albireo.as"

#include "blackHole.as"
#include "mystery.as"
#include "satellite1.as"
#include "satellite2.as"

class EBTemplate : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};