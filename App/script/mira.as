class BBMira : BulletBehavior{
    float speed;
    float accel;
    float radius;
    float time;

    BBMira(float pSpeed, float pAccel, float pRadius, float pTime){
        speed = pSpeed;
        accel = pAccel;
        radius = pRadius;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time)){
            data.angle += Mat3x2::Rotate(dtr(90)).transform(data.angle).normalized() * accel;
            data.pos += data.angle * speed;
            if(data.count > 4 * Math::Pi / Abs(accel))
                data.deleteFlag = true;
        }
    }
};

class Mira : EnemyBehavior{
    Texture tex("AS/star_1.png");
    BulletContainer c = GetBulletS(1, ColorF(1.0, 0.9, 0.8));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 2;
        while(m()){
            repeat k = 40;
            while(k()){
                BBMira b(3, sign(m) * (0.008 + 0.0005 * k), 60, 10);
                repeat i = 16;
                while(i()){
                    inter.RegisterBullet(c, BulletData(data.pos, radial(i) + sign(m) * dtr(k)).AutoDelete(), @b);
                }
                wait(10);
            }
            wait(300);
        }
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos, ColorF(1.0, 0.8, 0.5));
    }
};