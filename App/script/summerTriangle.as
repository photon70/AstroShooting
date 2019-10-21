class SummerTriangle : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    Point p1 = Point(90, -70);
    Point p2 = Point(-100, 65);
    Point p3 = Point(100, 75);
    BulletContainer c = GetBulletS(0.8, ColorF(0.9));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        repeat m = 360;
        while(m()){
            if(m.skip(2)){
                repeat i = 3;
                float r = shake(5);
                while(i()){
                    repeat k = 2;
                    while(k()){
                        float rad = r - m * dtr(37) + sign(k) * i * dtr(1);
                        inter.RegisterBullet(c, BulletData(data.pos + p2 + angle(rad) * 20, rad, 1 - 0.035 * i), @BBStraight(3));
                    }
                }
            }
            if(m.skip(15)){
                repeat i = 12;
                while(i()){
                    float rad = dtr(m) / 4 + radial(i);
                    inter.RegisterBullet(c, BulletData(data.pos + p3 + 20 * angle(rad), rad), BBStraight(2));
                }
            }
            if(m.skip(2)){
                repeat i = 3;
                while(i()){
                    float rad = Math::Pi * wavec(m, 360) + radial(i);
                    inter.RegisterBullet(c, BulletData(data.pos + p1 + 20 * angle(rad), rad), BBStraight(3.5));
                }
            }
            wait();
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.0).drawAt(data.pos + p1);
        tex.scaled(1.2).drawAt(data.pos + p2);
        tex.scaled(1.5).drawAt(data.pos + p3);
    }
};