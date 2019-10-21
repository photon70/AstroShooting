class BBElectro : BulletBehavior{
    float rad;
    int time;
    int num;

    BBElectro(float pRad, int pTime, int pNum = 20){
        rad = pRad;
        time = pTime;
        num = pNum;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        data.pos += rad * (EaseOutSine(float(data.count) / time) - EaseOutSine(float(data.count - 1) / time)) * data.angle;
        if(data.count >= time || data.deleteFlag){
            repeat m = num;
            while(m()){
                inter.RegisterBullet(GetBulletS(1, ColorF(1.0, 1.0, 0.8)), BulletData(data.pos, radial(m)), BBStraight(3));
            }
            data.deleteFlag = true;
        }
    }

    void Draw(const BulletData &inout data, const BulletContainer &inout c){
        int n = (data.count / 4) % 8;
        Circle(data.pos, c.rad)(c.texture(128 * n, 0, 128, 128)).draw(c.color);
    //    Circle(data.pos, c.rad).draw();
    }
};

BulletContainer GetBulletE(float size, ColorF color){
    BulletContainer b;
    b.texture = Texture(GetResource("AS/electro.png"));
    b.color = color;
    b.scale = 1.0;
    b.rad = size;
    b.score = 100;
    b.drawFlag = true;
    return b;
}

class Satellite1 : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    BulletContainer c = GetBulletE(30, ColorF(1.0, 1.0, 0.7));

    void Update(GameInterface &inout inter, EnemyData &inout data){
        int photo = inter.photoNum;

        Array<Float2> p = {
            Float2(0, -200),
            Float2(250, -200),
            Float2(0, -200),
            Float2(-250, -200),
            Float2(0, -200),
        };

        // repeat m = 8;
        // float r = shake();
        // while(m()){
        //     inter.RegisterBullet(c, BulletData(data.pos, r + radial(m) -dtr(180)), BBElectro(300, 90));
        // }
        repeat n = p.size() - 1;
        while(n()){

            if(photo == 0){
                wait(60);
                repeat m = 3;
                while(m()){
                    float rad = aim(data.pos, inter.player) + nway(m, dtr(20)) + shake(3);
                    inter.RegisterBullet(c, BulletData(data.pos + 10 * angle(rad), rad), BBElectro(200, 30));
                    wait(5);
                }
                wait(60);
            }
            else if(photo <= 3){
                wait(60);
                repeat k = 3;
                while(k()){
                    repeat m = 3;
                    while(m()){
                        float rad = aim(data.pos, inter.player) + nway(m, dtr(30)) + shake(3);
                        inter.RegisterBullet(c, BulletData(data.pos + 10 * angle(rad), rad), BBElectro(300 - 80 * k, 30 + 10 * k));
                        wait(5);
                    }
                    wait(10);
                }
                wait(60);
            }
            else if(photo <= 5){
                wait(60);
                repeat k = 4;
                while(k()){
                    repeat i = 2;
                    while(i()){
                        repeat m = 2;
                        while(m()){
                            float rad = aim(data.pos, inter.player) + sign(i) * (dtr(30) + nway(m, dtr(20))) + shake(3);
                            inter.RegisterBullet(c, BulletData(data.pos + 10 * angle(rad), rad), BBElectro(400 - 80 * k, 30 + 10 * k));
                            wait(5);
                        }
                    }
                        wait(10);
                }
                wait(60);
            }
            else if(photo <= 7){
                wait(60);
                repeat k = 50;
                while(k()){
                    float rad = dtr(18) * k + shake(18);
                    inter.RegisterBullet(c, BulletData(data.pos + 10 * angle(rad), rad), BBElectro(200 + 8 * k, 120));
                    wait(3);
                }
                wait(180);
            }

            repeat m = 30;
            while(m()){
                data.pos = Bezier(p[n], p[n + 1], EaseOutSine(m.progressO()));
                wait();
            }
            if(inter.photoNum > photo)
                photo = inter.photoNum;
        }
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};