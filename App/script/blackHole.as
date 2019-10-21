class BlackHole : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c1 = GetBulletS(1.0, ColorF(1.0, 0.8, 0.6));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        float s = shake();
        repeat m = 32;
        while(m()){
            repeat k = 12;
            while(k()){
                repeat i = 2;
                while(i()){
                    float rad = s + sign(i) * radialOut(m) / 2;
                    inter.RegisterBullet(c1, BulletData(data.pos + 80 * angle(rad), Math::HalfPi * wave(k, 10) + rad + shake(3), 1, k * 8), BBStraightAccel(0, 3, 0.013));
                }
            }
            wait(1);
        }
        wait(180);
    }

    void Draw(const EnemyData& data){
    //    tex.drawAt(data.pos);
    }
};