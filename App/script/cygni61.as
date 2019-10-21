class Cygni61 : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS(1.0, ColorF(1.0, 0.85, 0.7));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 16;
        while(m()){
            repeat i = 2;
            while(i()){
                repeat n = 4;
                    while(n()){
                    repeat k = 16;
                    float rad = radial(n) + sign(i) * dtr(m) * 12;
                    float s = shake();
                    while(k()){
                        inter.RegisterBullet(c, BulletData(data.pos + m * 15 * angle(rad), radial(k) + s), BBStraight(3));
                    }
                }
            }
            inter.aStar2 = true;
            wait(40);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.0).drawAt(data.pos, ColorF(1.0, 0.8, 0.6));
        tex.scaled(0.6).drawAt(data.pos + Point(-15, -10), ColorF(1.0, 0.8, 0.6));
    }
};