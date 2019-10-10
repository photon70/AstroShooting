class BBDelphinus : BulletBehavior{
    float accel;
    Locate locate;
    int time;

    BBDelphinus(float pAccel, Locate pLocate, int pTime){
    //    speed = pSpeed;
    //    radian = pRadian;
        accel = pAccel;
        locate = pLocate;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, locate, time)){
            data.angle += Mat3x2::Rotate(dtr(90)).transform(data.angle).normalized() * accel;
            data.pos += data.angle;
        }
    }
};

class Delphinus : EnemyBehavior{
    Texture tex("AS/star_1.png");
    BulletContainer c = GetBulletS();
    BulletContainer c2 = GetBulletM(0.8, ColorF(0.7, 0.95, 1.0));
    Array<Point> p = {
    Point(0, -15),
    Point(25, -0),
    Point(-5, 15),
    Point(-30, 5),
    Point(80, 40)};

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 72;
        while(m()){
            repeat i = 6;
            while(i()){
                repeat k = 4;
                while(k()){
                    auto rotate = m * dtr(-5);
                    auto rad = aim(p[k], p[(k+1)%4]) + rotate;
                    auto v = Mat3x2::Rotate(rotate).transform(p[k]);
                    inter.RegisterBullet(c, BulletData(data.pos + v * 6, rad, 1, 8*i), @BBCurve(2, sign(k)*i*0.001));
                }
            }
            repeat k = 10;
            while(k()){
                 inter.RegisterBullet(c2, BulletData(data.pos + p[4], nway(k, dtr(10)) + m * dtr(40) + shake(10)), @BBStraight(3));
            }
            wait(20);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.5).drawAt(data.pos + p[0], ColorF(0.8, 0.95, 1.0));
        tex.scaled(0.5).drawAt(data.pos + p[1], ColorF(1.0, 1.0, 1.0));
        tex.scaled(0.5).drawAt(data.pos + p[2], ColorF(0.9, 0.9, 0.9));
        tex.scaled(0.5).drawAt(data.pos + p[3], ColorF(1.0, 1.0, 0.7));
        tex.scaled(0.5).drawAt(data.pos + p[4], ColorF(0.7, 0.95, 1.0));
    }
};