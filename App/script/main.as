#include "bullet.as"
#include "enemy.as"

class TestEnemyBehavior : EnemyBehavior{
    int i = 0;
    BulletContainer c = GetBulletM();
    BBStraight b(3);
    Texture tex("AS/star_1.png");

    void Update(GameInterface &inout inter, EnemyData &inout data){
        if(i % 4 == 0)
            inter.RegisterBullet(c, BulletData(data.pos, i), @b);
        ++i;
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};

void ResEn(GameData &inout game,
    String &inout name,
    String &inout nickname,
    String &inout behavior,
    int timeLimit,
    int targetNumber,
    String &inout txt){
    EnemyContainer c;
    c.name = name;
    c.nickname = nickname;
    c.behaviorName = behavior;
    c.txt = txt;
    c.timeLimit = timeLimit;
    c.targetNumber = targetNumber;
    game.RegisterEnemy(c); 
}

void RegisterData(GameData &inout game){
    game.RegisterPage("秋の南の空");
    game.RegisterPage("秋の東の空");
    game.RegisterPage("秋の北の空");

    ResEn(game, "フォーマルハウト", "南のロイヤルスター", "Fomalhaut", 120, 3, "秋の南の空で一番輝く、\nフォーマルハウトは一等星。\n他の星の目印にもなるよ。");
    ResEn(game, "ペガスス座", "秋の四辺形","Pegusus", 120, 3, "明るいわけではないけれど、\n大きく目立つ四辺形は\n秋の夜空の道しるべです。");
    ResEn(game, "やぎ座アルゲディ", "二匹の仔山羊","Algedi", 120, 3, "アルゲディは仔山羊のイミ。\n普段は目立たぬ星だけど\n双眼鏡で探してみては？");
    ResEn(game, "いるか座", "ヒシボシ", "Delphinus", 120, 3, "夏の大三角の近くにある\nいるか座は、オタマジャクシ\nのほうが似てるかも。");
    ResEn(game, "みずがめ座三ツ矢", "Water Jar", "Waterjar", 120, 3, "三ツ矢はみずがめ座の\n水瓶(Water Jar)の位置。\n暗いから見つけにくいよ。");
    ResEn(game, "ペガスス座M15", "ペガススの鼻先", "M15", 120, 3, "M15はペガスス座の鼻先\nにある球状星団。\n赤っぽい星もおおいんだ。");

    ResEn(game, "くじら座ミラ", "怪物の心臓", "Mira", 180, 5, "海の怪物、ケ-トスの心臓は\n星座となった今もなお、\n脈動し続けている。");
    ResEn(game, "ペルセウス座アルゴル", "メデューサの首", "Algol", 180, 5, "ペルセウスが討ち取った\nメデューサの首の位置に\nアルゴルがあるよ。");
    ResEn(game, "牡羊座メサルティム", "一卵性双星児","Mesarthim", 180, 5, "ほとんど同じ大きさの星が\nすぐ近くに二つ並んでいる\nまるでうり二つの双子のよう。");
    ResEn(game, "ぎょしゃ座カペラ", "五角星の一角", "Capella", 180, 5, "カペラは五角星にあるけど、\n冬のダイヤモンドの六角形\nも形作っているんだ。");
    ResEn(game, "アンドロメダ銀河", "おとなりの銀河", "M31", 180, 5, "未設定です");
    ResEn(game, "おうし座すばる", "プレアデスの7姉妹","TestEnemyBehavior", 180, 5, "未設定です");

    ResEn(game, "ポラリス", "不動の星", "TestEnemyBehavior", 90, 5, "秋の南の空で一番輝く、\nフォーマルハウトは一等星。\n他の星の目印にもなるよ。");
    ResEn(game, "カシオペヤ座", "囚われの姫", "TestEnemyBehavior", 90, 5, "未設定です");
    ResEn(game, "ケフェウス座δ星", "星のものさし","TestEnemyBehavior", 90, 5, "未設定です");
    ResEn(game, "カシオペヤ座ET星団", "トンボ星団","TestEnemyBehavior", 90, 5, "未設定です");
    ResEn(game, "ガーネットスター", "ざくろ石の星", "TestEnemyBehavior", 90, 5, "未設定です");
    ResEn(game, "二重星団 h+χ", "宙の宝石箱", "TestEnemyBehavior", 90, 5, "未設定です");
}

void Loop(GameInterface &inout inter){
    
}