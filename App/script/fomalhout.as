class BBFomalhaut : BulletBehavior{
    float speed;
    float radius = 50;
    int time = 30;
    BulletContainer container = GetBulletS(1.0, ColorF(1.0, 0.95, 0.9));
    BBStraightAccel b(3, 2, -0.01);

    BBFomalhaut(float pSpeed){
        speed = pSpeed;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, radius, time))
            data.pos += data.angle * speed;
        if(data.count % 20 == 0 && data.count < 160){
            repeat i(3);
            BulletData buf(data.pos, data.radian + shake(5));
            while(i()){
                inter.RegisterBullet(container, buf.AddAngle(dtr(120)).SetCount(i*8), @b);
                inter.RegisterBullet(container, buf.AddAngle(-dtr(120)).SetCount(i*8), @b);
            }
        }
    }
};

class Fomalhaut : EnemyBehavior{
    BulletContainer c = GetBulletM(1.5);
    BBFomalhaut b(2);
    Texture tex("AS/star_1.png");

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat i = 20;
        auto r = aim(data.pos, inter.player);
        while(i()) {
            // BBStraightAccel b(0, 5, 0.03);
            // if(i % 2 == 0)
            //     inter.RegisterBullet(c, BulletData(data.pos, ToRadians(i * 9)), @b);
            inter.RegisterBullet(c, BulletData(data.pos, radial(i.max, r, i)), @b);
        }
        wait(300);
    }

    void Draw(const EnemyData& data){
        tex.scaled(1.5).drawAt(data.pos);
    }
};