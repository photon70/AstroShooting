class M13 : EnemyBehavior{
//    Texture tex(GetResource("AS/star_1.png"));
BulletContainer c = GetBulletS();

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        repeat i = 360;
        while(i()){
            repeat k = 8;
            while(k()){
                inter.RegisterBullet(c, BulletData(data.pos, radial(k) + i * dtr(13) + shake(5)), @BBStraightAccel(0.5, 2 + rand(2), 0.03));
            }
            wait(2);
        }
    }

    void Draw(const EnemyData& data){
//        tex.drawAt(data.pos);
    }
};