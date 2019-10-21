class DoubleCluster : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    Array<BulletContainer> c = {
        GetBulletS(1.0),
        GetBulletS(1.0, HSV(0, 0.05, 1.0)),
        GetBulletS(1.0, HSV(30, 0.05, 1.0)),
        GetBulletS(1.0, HSV(60, 0.05, 1.0)),
        GetBulletS(1.0, HSV(90, 0.05, 1.0)),
        GetBulletS(1.0, HSV(90, 0.1, 1.0)),
        GetBulletS(1.0, HSV(180, 0.05, 1.0)),
        GetBulletS(1.0, HSV(180, 0.1, 1.0)),
        GetBulletS()
    };

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        repeat i = 360;
        while(i()){
            repeat k = 8;
            while(k()){
                inter.RegisterBullet(c[k], BulletData(data.pos + Point(100, 0), radial(k) + i * 27 + shake(5)), @BBStraightAccel(1, 2, 0.05));
                inter.RegisterBullet(c[k], BulletData(data.pos + Point(-100, 0), radial(k) + -i * 27 + shake(5)), @BBStraightAccel(1.5, 3, 0.1));
            }
            wait(6);
        }
    }

    void Draw(const EnemyData& data){
    //    tex.drawAt(data.pos);
    }
};