#include "player.h"
#include "assets.h"


const float DOWN_FORCE = 2000.0f;
const float UP_FORCE = 35.f;
const float SIDES_FORCE = 5.0f;

Player::Player(GameScene* _scene, char _letter, vec2f pos)
    : Actor(_scene)
    , scene(_scene)
    , letter(_letter)
    , jumping(100)
    , notinground(0)
    , score(0)
    , inground(0)
    , normal(0,1)
{

    b2CircleShape circle;
    circle.m_radius = 1.f;

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(pos.x, pos.y);
    b2Body* body = scene->world.CreateBody(&def);
    b2FixtureDef fixtureDef;
    fixtureDef.friction = 0.05;
    fixtureDef.shape = &circle;
    fixtureDef.density = 3;
    body->CreateFixture(&fixtureDef);
    body->SetAngularVelocity(-60.f);
    this->setBody(body);
}

float frand(float f) {
    return ((float(rand()) / float(RAND_MAX)) * 2 - 1) * f;
}

float frandp(float f) {
    return (float(rand()) / float(RAND_MAX)) * f;
}

vec2f frandv(float f) {
    return vec2f(frand(f), frand(f));
}
vec2f frandv(float x, float y) {
    return vec2f(frand(x), frand(y));
}

void Player::update() {
    input = scene->getPlayerInput(letter);

/*
    if (scene->winner) {
        auto v = body->GetLinearVelocity();
        v *= 0.95f;
        body->SetLinearVelocity(v);
        return;
    }
*/


    b2ContactEdge* e = body->GetContactList();
    int ct = 0;
    b2Vec2 lol(0, 0);
    while(e != nullptr) {
        if(e->contact->IsTouching()) {
            b2WorldManifold wm;
            e->contact->GetWorldManifold(&wm);
            b2Vec2 n = wm.normal;
            b2Vec2 d = body->GetPosition() - wm.points[0]; // lolol
            float dotito = b2Dot(n, d);
            if(dotito < 0) n = -1.0f * n;
            lol += n;
            ct++;
        }
        e = e->next;
    }

    notinground++;
    inground++;
    if(ct != 0) {
        notinground = 0;
        normal = (1.0f / ct) * lol;
        normal.Normalize();
    } else {
        inground = notinground > 5 ? 0 : 100;
    }

    vec2f n = vec2f(normal.x, normal.y);
    vec2f n2 = vec2f(-normal.y, normal.x);

    jumping++;
    if(input == WebSocketInput::UP && jumping > 20 && notinground < 6) {
        body->SetLinearVelocity(body->GetLinearVelocity() + UP_FORCE * normal);
        jumping = 0;

        for(int i = 0; i < 5; i++) {
            Particle p;
            p.startCol = p.endCol = vec4f(1, 1, 1, 1);
            p.endCol.a = 0;
            p.startSize = 0.4;
            p.endSize = 0.2;
            p.life = 0.5;
            p.texIndex = 1;
            p.v = n * frand(1) + n2 * frand(8);
            p.p = getPosition() - n + frandv(0.3);
            scene->particles->addParticle(p);
        }
    }

    if(input == WebSocketInput::DOWN) {
        body->ApplyForceToCenter(b2Vec2(0, -DOWN_FORCE), true);
        if(notinground < 4) {
            for(int i = 0; i < 5; i++) {
                Particle p;
                p.startCol = p.endCol = vec4f(1,1,1, 1);
                p.endCol.a = 0;
                p.startSize = 0.3;
                p.endSize = 0.1;
                p.life = 0.4;
                p.texIndex = 1;
                float direction = n.x > 0 ? 1 : -1;
                p.v = n * frand(1) + direction * n2 * (5+frandp(7));
                p.p = getPosition() - n + frandv(0.1) + n2*frand(0.6);
                scene->particles->addParticle(p);
            }
        }
    }

    float distance = getPositionVec3().x-scene->br.x;

    float communism = 35.f;
    communism += fabs(distance*10.f);
    body->ApplyForceToCenter(b2Vec2(communism,0.f), true);

    static vec4f cols[] = {
        vec4f(1, 0, 0, 1),
        vec4f(1, 1, 0, 1),
        vec4f(0, 1, 0, 1),
        vec4f(0, 1, 1, 1),
        vec4f(0, 0, 1, 1),
    };

    vec2f pos = getPosition();
    if(scene->winner == letter) {
        for(float j = 0; j < 1; j+=0.1) {
            for(int i = 0; i < 5; i++) {
                Particle p;
                p.startCol = p.endCol = cols[i];
                p.endCol.a = 0;
                p.startSize = 0.3;
                p.endSize = 0;
                p.life = 1;
                p.p = pos * j + oldPos * (1-j);
                p.p.y += (i-2) * 0.3;
                scene->particles->addParticle(p);
            }
        }
    }
    oldPos = pos;
}

void Player::draw() {
    Texture2D* face = faces[faceIndex(letter)];
    vec2f pos = getPosition();

    vec2f squashDir(normal.x, normal.y);
    float squash = 1.f;
    squash += sin(jumping*0.4)*exp(-jumping*0.07) * 0.4f;
    squash -= sin(inground*0.5)*exp(-inground*0.12) * 0.2f;
    if(input == WebSocketInput::DOWN)
        squash -= 0.16;

    glm::rotate(mat3f(1.0f), 1.0f);
    quadShader.uniform("u_tex")->set(face);
    mat3f lol = projection;
    lol = glm::translate(lol, pos);
    lol = glm::scale(lol, vec2f(1.6f, 1.6f));
    lol *= mat3f(
                squashDir.x, squashDir.y, 0,
                -squashDir.y, squashDir.x, 0,
                0, 0, 1
    );
    lol = glm::translate(lol, vec2f(-1.f+squash, 0.f));
    lol = glm::scale(lol, vec2f(squash, 1/squash));
    lol *= mat3f(
                squashDir.x, -squashDir.y, 0,
                squashDir.y, squashDir.x, 0,
                0, 0, 1
    );
    lol = glm::rotate(lol, getAngle()/6);
    quadShader.uniform("mvp")->set(lol);
    quadMesh.draw(quadShader);

}
