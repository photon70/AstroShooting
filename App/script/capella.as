class BBCapella : BulletBehavior{
    float speed;
    float accel;
    float radius;
    float time;

    BBCapella(float pSpeed, float pAccel, float pRadius, float pTime){
        speed = pSpeed;
        accel = pAccel;
        radius = pRadius;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.pos += Mat3x2::Rotate(dtr(90)).transform(data.angle).normalized() * accel;
        if(arrange(data, radius, time)){
            data.pos += data.angle * speed;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};
class Capella : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS();
    BulletContainer c2 = GetBulletS(1.2, ColorF(1.0, 1.0, 0.9));
    Array<Point> p = {Point(-200, -150), Point(0, -200), Point(200, -150)};

    void Update(GameInterface &inout inter, EnemyData &inout data){
        inter.aStardust = true;
        repeat k = 360;
        while(k()){
            repeat m = 3;
            if(k.skip(3)){
                while(m()){
                    inter.RegisterBullet(c, BulletData(data.pos + p[m], dtr(47) * k + shake(10)), @BBStraightAccel(0.5, 3.5, 0.05));
                }
            }

            m = 5;
            if(k.skip(80)){
                float r = shake(10);
                while(m()){
                    repeat i = 15;
                    while(i()){
                        inter.RegisterBullet(c2, BulletData(data.pos, radial(m) + r + dtr(k * 3)), @BBCapella(2, 0.1 * i.center(), 80, 20));
                    }
                }
                inter.aStar2 = true;
            }

            wait();
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.5).drawAt(data.pos, ColorF(1.0, 1.0, 0.8));
    }
};