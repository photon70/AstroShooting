class Waterjar : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS(1.0, ColorF(0.7, 0.9, 1.0));
    BulletContainer c2 = GetBulletS(1.0, ColorF(0.8, 0.9, 1.0));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust= true;
        repeat i = 3;
        while(i()){
            auto rad = radial(i) + aim(data.pos, inter.player);
            repeat k = 8;
            while(k()){
                repeat m = 3;
                while(m()){
                    inter.RegisterBullet(c, BulletData(data.pos, rad + dtr(k*2), 1.0, k*1 + m * 10), BBStraightAccelA(1, 6, 0.05, 80, 10));
                    inter.RegisterBullet(c, BulletData(data.pos, rad + dtr(k*-2), 1.0, k*1 + m* 10), BBStraightAccelA(1, 6, 0.05, 80, 10));
                }
            }
        }
        inter.aStar1 = true;
    //    wait(120);
        i = 10;
        while(i()){
            repeat k = 4;
            while(k()){
                inter.RegisterBullet(c2, BulletData(Bezier(Float2(-300, -450), Float2(300, -450), k.progressO()), dtr(180) + shake(5) + dtr(20) * wave(i, 10)), @BBStraight(2));
            }
            wait(16);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.5).drawAt(data.pos, ColorF(1.0, 1.0, 0.9));
        tex.scaled(0.5).drawAt(data.pos + Point(10, -15), ColorF(0.7, 0.9, 1.0));
        tex.scaled(0.5).drawAt(data.pos + Point(15, 17), ColorF(1.0, 1.0, 1.0));
        tex.scaled(0.5).drawAt(data.pos + Point(-17, -2), ColorF(0.8, 0.9, 1.0));
    }
};