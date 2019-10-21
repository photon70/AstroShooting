class M15 : EnemyBehavior{
//    Texture tex("AS/star_1.png");
BulletContainer c = GetBulletS();
BulletContainer c2 = GetBulletM(1.2, ColorF(1.0, 0.9, 0.7));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        int i = 0;
        while(true){
            repeat k = 8;
            while(k()){
                inter.RegisterBullet(c, BulletData(data.pos, radial(k) + i * 27 + shake(3)), @BBStraightAccel(0.5, 4, 0.01));
            }
            ++i;
            if(i % 20 == 0){
                repeat m = 16;
                while(m()){
                    inter.RegisterBullet(c2, BulletData(data.pos, radial(m) + aim(data.pos, inter.player)), @BBStraight(4.2));
                }
            inter.aStar2 = true;
            }
            wait(3);
        }
    }

    void Draw(const EnemyData& data){
        
    }
};