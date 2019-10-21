class BBAlgol : BulletBehavior{
    float speed;
    float accel;
    float time;
    bool flag = false;

    BBAlgol(float pSpeed, float pAccel, int pTime){
        speed = pSpeed;
        accel = pAccel;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(!flag){
            data.angle += Mat3x2::Rotate(dtr(90)).transform(data.angle).normalized() * accel;
            data.pos += data.angle * speed;
        }
        if(float(data.count) > Math::TwoPi / Abs(accel) + time)
            data.deleteFlag = true;
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class Algol : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletS();
    BulletContainer c2 = GetBulletS(0.7, ColorF(1.0, 0.9, 0.7));
    BBStraight bs(2.5);

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 2;
        while(m()){
            inter.aStar2 = true;
            inter.aStardust = true;
            float r = shake();
            repeat k = 80;
            while(k()){
                BBAlgol b(6, sign(m) * (0.015 + 0.0003*k), 0);
                repeat i = 12;
                while(i()){
                    inter.RegisterBullet(c, BulletData(data.pos, r + radial(i)).AutoDelete(), @b);
                }
                i = 8;
                while(i()){
                    inter.RegisterBullet(c2, BulletData(data.pos, radial(i) + shake(5) + -sign(m) * k * 3), bs);
                }
                wait(10);
            }
            inter.aStar1 = true;
            k = 16;
            while(k()){
                BBAlgol b(4,  sign(m) * (0.07 + 0.0003*k), 0);
                repeat i = 20;
                while(i()){
                    inter.RegisterBullet(c, BulletData(data.pos, radial(i)).AutoDelete(), @b);
                }
                wait(5);
            }
            wait(60);
            inter.aStardust = false;
        }
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};