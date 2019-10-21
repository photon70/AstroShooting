class BBFomalhaut : BulletBehavior{
    float speed;
    float radius = 50;
    int time = 30;
    int endTime;
    BulletContainer container = GetBulletS(1.0, ColorF(1.0, 0.95, 0.9));
    BBStraightAccel b(3, 2, -0.01);

    BBFomalhaut(float pSpeed, float pEndTime = 160){
        speed = pSpeed;
        endTime = pEndTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time))
            data.pos += data.angle * speed;
        if(data.count % 20 == 0 && data.count < endTime){
            repeat i(3);
            BulletData buf(data.pos, data.radian + shake(5));
            inter.aStar2 = true;
            while(i()){
                inter.RegisterBullet(container, buf.AddAngle(dtr(120)).SetCount(i*8), @b);
                inter.RegisterBullet(container, buf.AddAngle(-dtr(120)).SetCount(i*8), @b);
            }
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBFomalhaut2 : BulletBehavior{
    float speed;
    float radius = 50;
    int time = 30;
    int endTime;
    BulletContainer container = GetBulletS(1.0, ColorF(1.0, 0.95, 0.9));

    BBFomalhaut2(float pSpeed, float pEndTime = 160){
        speed = pSpeed;
        endTime = pEndTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time))
            data.pos += data.angle * speed;
        if(data.count % 20 == 0 && data.count < endTime){
            repeat i(3);
            BulletData buf(data.pos, data.radian + shake(5));
            inter.aStar2 = true;
            while(i()){
                inter.RegisterBullet(container, buf.AddAngle(dtr(120)).SetCount(i*8), BBFomalhaut3(3, 60 - i * 8, 30));
                inter.RegisterBullet(container, buf.AddAngle(-dtr(120)).SetCount(i*8), BBFomalhaut3(3, 60 - i * 8, 30));
            }
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class BBFomalhaut3 : BulletBehavior{
    float speed;
    int stopTime;
    int waitTime;

    BBFomalhaut3(float pSpeed, int pStopTime, int pWaitTime){
        speed = pSpeed;
        stopTime = pStopTime;
        waitTime = pWaitTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(data.count < stopTime)
            data.pos += data.angle * speed;
        if(data.count > stopTime + waitTime)
            data.deleteFlag = true;
    }

    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

class Fomalhaut : EnemyBehavior{
    BulletContainer c = GetBulletM(1.2);
    Texture tex(GetResource("AS/star_1.png"));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        int photo = inter.photoNum;

        auto r = aim(data.pos, inter.player);
        if(photo == 0){
            repeat i = 10;
            while(i()) {
                inter.aStar1 = true;
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(i.max, r, i)), BBStraight(2));
            }
            wait(60);
        }
        else if(photo == 1){
            BBFomalhaut b(2, 40);
            repeat i = 10;
            while(i()) {
                inter.aStar1 = true;
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(i.max, r, i)), @b);
            }
            wait(100);
        }
        else if(photo == 2){
            BBFomalhaut2 b(2, 200);
            repeat i = 16;
            while(i()) {
                inter.aStar1 = true;
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(i.max, r, i)), @b);
            }
            wait(100);
        }
        else if(photo == 3){
            BBFomalhaut2 b(2, 240);
            repeat i = 20;
            while(i()) {
                inter.aStar1 = true;
                inter.RegisterBullet(c, BulletData(data.pos, radialOut(i.max, r, i)), @b);
            }
            wait(100);
        }
        else {
            BBFomalhaut b(2);
            repeat i = 20;
            while(i()) {
                inter.aStar1 = true;
                inter.RegisterBullet(c, BulletData(data.pos, radial(i.max, r, i)), @b);
            }
            wait(300);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.5).drawAt(data.pos);
    }
};