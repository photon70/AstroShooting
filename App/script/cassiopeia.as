class BBCassiopeia : BulletBehavior {
    float speed;
    float maxSpeed;
    float accel;
    float rad;

    BBCassiopeia(float pSpeed, float pMaxSpeed, float pAccel, float pRad){
        speed = pSpeed;
        maxSpeed = pMaxSpeed;
        accel = pAccel;
        rad = pRad;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.pos += data.angle * speed;
        speed += accel;
        if(speed < 0){
            speed = 0;
            accel = -accel;
            data.angle = Mat3x2::Rotate(rad).transform(data.angle);
            rad = -rad;
        }
        if(maxSpeed < speed){
            speed = maxSpeed;
            accel = -accel;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class Cassiopeia : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    Array<BulletContainer> c = {
        GetBulletS(1.0, ColorF(1.0, 1.0, 0.9)),
        GetBulletS(1.0, ColorF(0.93, 0.98, 1.0)),
        GetBulletS(1.0)
    };

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat n = 360;
        while(n()){
            inter.aStar2 = true;
            repeat m = 8;
            while(m()){
                repeat k = 16;
                while(k()){
                    float rad = radial(k) + sign(n) * dtr(n) * -3;
                    inter.RegisterBullet(c[n % 2], BulletData(data.pos + angle(rad + sign(n) *  dtr(60)) * 80, rad), @BBCassiopeia(0, 5, 0.1, sign(n) * dtr(40)));
                }
                wait(4);
            }
            if(n % 5 == 0){
            inter.aStardust = true;
                m = 20;
                while(m()){
                    repeat k = 12;
                    while(k()){
                        inter.RegisterBullet(c_bs, BulletData(data.pos, radial(k) + m * dtr(10) + shake(5), 1,  m * 2), @BBStraightAccel(1, 3, 0.03));
                    }
                }
            }
            if(n % 5 == 1)
                inter.aStardust = false;
            wait(10);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.5).drawAt(data.pos + Point(-20, -48));
        tex.scaled(0.5).drawAt(data.pos + Point(18, -38), ColorF(1.0, 1.0, 0.85));
        tex.scaled(0.5).drawAt(data.pos + Point(0, 0), ColorF(0.85, 0.95, 1.0));
        tex.scaled(0.5).drawAt(data.pos + Point(20, 23));
        tex.scaled(0.5).drawAt(data.pos + Point(5, 60), ColorF(0.75, 0.9, 1.0));
    }
};