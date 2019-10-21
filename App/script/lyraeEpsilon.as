class LyraeEpsilon : EnemyBehavior{
    Texture tex(GetResource("AS/star_1.png"));
    Point p(60, 0);
    BulletContainer c = GetBulletS();

    void Update(GameInterface &inout inter, EnemyData &inout data){
        // repeat i = 48;
        // float s = shake();
        // while(i()){
        //     repeat k = 2;
        //     while(k()){
        //         float rad = radial(i) + s + shake(0.5);
        //         inter.RegisterBullet(c, BulletData(data.pos + sign(k) * p + 20 * angle(rad), rad), BBStraight(2));
        //         rad = radialOut(i) + s + shake(0.5);
        //         inter.RegisterBullet(c, BulletData(data.pos + sign(k) * p + 20 * angle(rad), rad, 1.0, 20), BBStraight(2.15));
        //     }
        // }
        // inter.aStar2 = true;
        // wait(20);
        // inter.aStar2 = true;
        // wait(40);

            
            repeat k = 2;
            while(k()){
                repeat i = 48;
                float s = shake();
                while(i()){
                    float rad = radial(i) + s + shake(0.5);
                    inter.RegisterBullet(c, BulletData(data.pos + sign(k) * p + 20 * angle(rad), rad), BBStraight(1.5));
                    rad = radialOut(i) + s + shake(0.5);
                    inter.RegisterBullet(c, BulletData(data.pos + sign(k) * p + 20 * angle(rad), rad, 1.0, 20), BBStraight(2));
                }
                inter.aStar2 = true;
                wait(20);
                inter.aStar2 = true;
                wait(20);
            }
    }

    void Draw(const EnemyData& data){
        tex.scaled(0.5).drawAt(data.pos + p + Point(0, 3));
        tex.scaled(0.5).drawAt(data.pos + p + Point(0, -3));
        tex.scaled(0.3).drawAt(data.pos - p + Point(-3, 0));
        tex.scaled(0.5).drawAt(data.pos - p + Point(3, 0));
    }
};