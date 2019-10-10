class Mesarthim : EnemyBehavior{
    Texture tex("AS/star_1.png");
    Array<Point> p = {Point(5, 0), Point(-5, 0)};
    Array<BulletContainer> c = {
        GetBulletS(1, ColorF(0.95, 1.0, 0.98)), 
        GetBulletS(1, ColorF(1.0, 1.0, 0.95)), 
        GetBulletS(1, ColorF(1.0, 1.0, 0.95))};

    void Update(GameInterface &inout inter, EnemyData &inout data){
        repeat k = 360;
        while(k()){
        repeat m = 2;
        while(m()){
            repeat i = 12;
            while(i()){
                float rad = sign(m) * k * 1.5 + radial(i) + wavec(k, 360, dtr(90));
               inter.RegisterBullet(c[m], BulletData(data.pos + p[m] + angle(rad) * 50, rad), @BBStraightAccel(0.5, 2.5, 0.01));
            }
            Print << m.count;
        }
        wait(8);
        }
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.5).drawAt(data.pos + p[0], ColorF(0.94, 1.0, 0.98));
        tex.scaled(0.5).drawAt(data.pos + p[1], ColorF(1.0, 1.0, 0.95));
    }
};