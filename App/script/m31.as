class M31 : EnemyBehavior{
    Texture tex("AS/m31.png");
    BulletContainer c = GetBulletS();

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 360;
        while(m()){
            repeat k = 2;
            while(k()){
                float r = radial(k) + dtr(m) * 3;
                inter.RegisterBullet(c, BulletData(data.pos + angle(r) * 60, r), @BBStraight(3));
            }
            wait(1);
        }
    }

    void Draw(const EnemyData& data){
        tex.rotated(dtr(-45)).drawAt(data.pos);
    }
};