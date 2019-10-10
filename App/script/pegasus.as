class BBPegasus : BulletBehavior{
    float speed;
    float rad;
    float radius;
    int time;

    BBPegasus(float pRad,  float pRadius, int pTime){
        rad = pRad;
        radius = pRadius;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time)){
            repeat i = 3;
            float buf = shake(3);
            while(i()){
                inter.RegisterBullet(c_bs, BulletData(data.pos, data.radian + dtr(rad) + buf, 1.0, i * 6), @BBStraightAccel(6, 1, -0.03));
            }
            data.deleteFlag = true;
        }
    }
}

class BBPegasus2 : BulletBehavior{
    float speed;
    Locate locate;
    int time;
    int waitTime;

    BBPegasus2(float pSpeed, int pWaitTime,  Locate pLocate, int pTime){
        speed = pSpeed;
        waitTime = pWaitTime;
        locate = pLocate;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, locate, time)){
            if(data.count > time + waitTime){
                data.pos += data.angle * speed;
            }
        }
    }
}

class Pegusus : EnemyBehavior{
    BulletContainer c2 = GetBulletS(1);
    BulletContainer c3 = GetBulletM(0.6, ColorF(0.9, 0.8, 0.6));
    Texture tex("AS/star_1.png");
    Float2 p1(95, -70);
    Float2 p2(100, 70);
    Float2 p3(-100, 75);
    Float2 p4(-80, -80);

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat k = 12;
        while(k()){
            repeat i = 4;
            while(i()){
                inter.RegisterBullet(c2, BulletData(data.pos + Point(0, -100), nway(k, dtr(80), dtr(8))), @BBPegasus(120, Bezier(50, 500, i.progress()), 30));
                inter.RegisterBullet(c2, BulletData(data.pos + Point(0, -100), nway(k, -dtr(80), -dtr(8))), @BBPegasus(-120, Bezier(50, 500, i.progress()), 30));
            }
            wait(15);
        }
        k = 12;
        while(k()){
            repeat i = 4;
            while(i()){
                inter.RegisterBullet(c2, BulletData(data.pos + Point(0, -100), nway(k, dtr(80), -dtr(8))), @BBPegasus(120, Bezier(50, 500, i.progress()), 30));
                inter.RegisterBullet(c2, BulletData(data.pos + Point(0, -100), nway(k, -dtr(80), dtr(8))), @BBPegasus(-120, Bezier(50, 500, i.progress()), 30));
            }
            wait(15);
        }
        wait(30);

        k = 30;
        while(k()){
                inter.RegisterBullet(c3, BulletData(data.pos + p1, radial(k, shake(10))), @BBPegasus2(4, 40, Locate(p1, Bezier(p1, p2, k.progress())), 60));
                inter.RegisterBullet(c3, BulletData(data.pos + p2, radial(k, shake(10))), @BBPegasus2(4, 40, Locate(p2, Bezier(p2, p3, k.progress())), 60));
                inter.RegisterBullet(c3, BulletData(data.pos + p3, radial(k, shake(10))), @BBPegasus2(4, 40, Locate(p3, Bezier(p3, p4, k.progress())), 60));
                inter.RegisterBullet(c3, BulletData(data.pos + p4, radial(k, shake(10))), @BBPegasus2(4, 40, Locate(p4, Bezier(p4, p1, k.progress())), 60));

        }
        wait(100);
    }

    void Draw(const EnemyData& data) {
        tex.scaled(1).drawAt(data.pos + p1, ColorF(1.0, 0.6, 0.3));
        tex.scaled(1).drawAt(data.pos + p2);
        tex.scaled(1).drawAt(data.pos + p3, ColorF(0.8, 0.9, 1.0));
        tex.scaled(1).drawAt(data.pos + p4);
    }
};