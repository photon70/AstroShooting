class Albireo : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c1 = GetBulletS(1.0, ColorF(1.0, 0.95, 0.8));
    BulletContainer c2 = GetBulletS(1.0, ColorF(0.9, 0.95, 1.0));
    Point p(10, -20);

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        float d = dtr(2);
        repeat n = 360;
            while(n()){
            float s = shake(7);
            repeat m = 10;
            while(m()){
                repeat k = 12;
                while(k()){
                    float rad = radial(k) + s + sign(n) *  m * d + n * dtr(15);
                    if(sign(n) < 0)
                        rad += m.max * d;
                    inter.RegisterBullet(c1, BulletData(data.pos + 40 * angle(rad), rad, 1.0, m * 3), BBStraight(2.5));
                }
            }

            if(n.skip(3)){
                float rad = aim(data.pos + p, inter.player);
                repeat i = 8;
                while(i()){
                    repeat m = 5;
                    while(m()){
                        repeat k = 2;
                        while(k()){
                            float r = rad + radial(i) + sign(k) * dtr(m) * 1.5;
                            inter.RegisterBullet(c2, BulletData(data.pos + p + 10 * angle(r), r, 1.0, m * 2), BBStraight(3.5));
                        }
                    }
                }
                inter.aStar2 = true;
            }

            wait(30);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.0).drawAt(data.pos, ColorF(1.0, 0.9, 0.7));
        tex.scaled(0.7).drawAt(data.pos + p, ColorF(0.85, 0.95, 1.0));
    }
};