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

    if(input == WebSocketInput::DOWN)
        body->ApplyForceToCenter(b2Vec2(0, -DOWN_FORCE), true);

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

    jumping++;
    if(input == WebSocketInput::UP && jumping > 20) {
        if(notinground < 6) {
            body->SetLinearVelocity(body->GetLinearVelocity() + UP_FORCE * normal);
            jumping = 0;
        }
    }

    float distance = getPositionVec3().x-scene->br.x;

    float communism = 35.f;
    communism += fabs(distance*10.f);
    body->ApplyForceToCenter(b2Vec2(communism,0.f), true);
}

void Player::draw() {
    Texture2D* face = faces[faceIndex(letter)];
    vec2f pos = getPosition();

    vec2f squashDir(normal.x, normal.y);
    float squash = 1.f;
    squash += sin(jumping*0.4)*exp(-jumping*0.07) * 0.4f;
    squash += sin(inground*0.5)*exp(-inground*0.12) * 0.2f;
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
