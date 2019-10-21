class M57 : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletM(1.2);

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 6;
        float s = shake();
        inter.aStardust = true;
        while(m()){
            repeat k = 28;
            while(k()){
                repeat i = 2;
                while(i()){
                    float rad = s + radial(k) + sign(i) * dtr(m) * 4;
                    inter.RegisterBullet(c, BulletData(data.pos, rad), BBCurveSAW(2.5, sign(i) * 0.003, 60 + 10 * (m.max - m), 30, 80));
                    inter.RegisterBullet(c, BulletData(data.pos, rad + dtr(180.0 / float(k.max))), BBCurveSAW(2.5, sign(i) * 0.003, -60 - 10 * m, 30, 80));
                }
            }
            wait(8);
        }
        inter.aStardust = false;
        wait(62);
        inter.aStar1 = true;
        wait(118);
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.3).drawAt(data.pos);
    }
};