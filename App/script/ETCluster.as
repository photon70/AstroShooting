class BBETCluster : BulletBehavior{
    float speed;
    float maxSpeed;
    float accel;
    Float2 vec;
    float time;

    BBETCluster(float pSpeed, float pMaxSpeed, float pAccel, Float2 pVec, float pTime){
        speed = pSpeed;
        maxSpeed = pMaxSpeed;
        accel = pAccel;
        vec = pVec;
        time = pTime;
    }

    void Update(GameInterface &inout inter, BulletData &inout data){
        if(arrange(data, vec, time)){
            data.pos += data.angle * speed;
            speed += accel;
            if(accel > 0) if(speed > maxSpeed)speed = maxSpeed;
            if(accel < 0) if(speed < maxSpeed)speed = maxSpeed;
        }
    }
    void Draw(const BulletData &inout data, const BulletContainer &inout c){}
};

int Lcm(int x, int y){
    int ret = x * y;
    bool flag = true;
    while(flag){
        flag = false;
        if(x % 2 == 0 && y % 2 == 0){
            x /= 2;
            y /= 2;
            ret /= 2;
            flag = true;
        }
        if(x % 3 == 0 && y % 3 == 0){
            x /= 3;
            y /= 3;
            ret /= 3;
            flag = true;
        }
        if(x % 5 == 0 && y % 5 == 0){
            x /= 5;
            y /= 5;
            ret /= 5;
            flag = true;
        }
        if(x % 7 == 0 && y % 7 == 0){
            x /= 7;
            y /= 7;
            ret /= 7;
            flag = true;
        }
    }
    ret /= x;
    return ret * 2;
}

class ETCluster : EnemyBehavior{
    Texture tex(GetResource("AS/et_cluster.png"));
    Array<Float2> vec;

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat m = 7;
        while(m()){
            repeat n = 6;
            while(n()){
                float x = m + 1;
                float y = n + 1;
                if(x <= y)
                    ++y;

                repeat k = 360;
                Float2 last(-1, 0);
                Float2 current;
                int p = Lcm(x, y);
                inter.aStar2 = true;
                inter.aStardust = true;
                while(k()){
                    float r = dtr(k * p);
                    float rad = 120.0 * Sin(r * x / y);
                    current = rad * angle(r);
                    inter.RegisterBullet(c_bs, BulletData(data.pos, aim(last, current) + shake(10)), BBETCluster(0, 3 + shake(1), 0.05, angle(r) * rad, 20));
                    last = current;
                }
                wait(30);
                inter.aStardust = false;
                inter.aStar1 = true;
                wait(70);
            }
        }
    }

    void Draw(const EnemyData& data){
        tex.drawAt(data.pos);
    }
};