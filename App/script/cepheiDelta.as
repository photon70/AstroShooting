class CepheiDelta : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c1 = GetBulletS(1.0, ColorF(1.0, 0.95, 0.9));
    BulletContainer c2 = GetBulletS(1.0, ColorF(0.9, 0.9, 1.0));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        repeat n = 360;
        while(n()){
            repeat m = 5;
            float r = shake(5);
            while(m()){
                repeat k = 2;
                while(k()){
                    float rad = r + n * dtr(34) + sign(k) * m * dtr(2);
                    inter.RegisterBullet(c1, BulletData(data.pos + angle(rad) * 40, rad, 1 - 0.035 * m), @BBStraight(3));
                }
            }
            if(n % 60 == 0)
                inter.aStar1 = true;
            if(n % 60 < 30){
                repeat k = 4;
                while(k()){
                    inter.RegisterBullet(c2, BulletData(data.pos + Point(-5, -20), radial(k) + -dtr(25) * n + shake(5)), @BBStraightAccel(1, 3.5, 0.05));
                }
            }
            if(n % 60 == 30)
            inter.aStar2 = true;

            wait(3);
        }
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos, ColorF(1.0, 0.95, 0.9));
        tex.scaled(0.6).drawAt(data.pos + Point(-5, -20), ColorF(0.8, 0.8, 1.0));
    }
};