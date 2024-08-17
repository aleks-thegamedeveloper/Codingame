#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <tgmath.h>
#include <limits.h>

using namespace std;

/**
 * Save the Planet.
 * Use less Fossil Fuel.
 **/

float deg2rad(float deg) {
    return deg * M_PI / 180.f;
}

float rad2deg(float rad) {
    return rad * 180.f / M_PI;
}

bool isNearlyZero(float f) {
    return abs(f - 0.f) < 1e-4;
}

struct vec2f
{
    void operator+= (const vec2f v) {
        x += v.x;
        y += v.y;
    }

    friend vec2f operator* (const vec2f& v, float k) {
        return { v.x * k, v.y * k };
    }

    friend vec2f operator+ (const vec2f& a, const vec2f& b) {
        return { a.x + b.x, a.y + b.y };
    }

    friend vec2f operator- (const vec2f& a, const vec2f& b) {
        return { a.x - b.x, a.y - b.y };
    }

    static vec2f rotate(const vec2f& vec, float deg) {
        const float theta { deg2rad(deg) };

        const float cs { cos(theta) };
        const float sn { sin(theta) };

        return { vec.x * cs - vec.y * sn, vec.x * sn + vec.y * cs };
    }

    static float mag(const vec2f& v) {
        return sqrt(v.x * v.x + v.y * v.y);
    }

    static vec2f getNormal(const vec2f& vec) {
        const float mag { vec2f::mag(vec) };
        if(isNearlyZero(mag))
            return {};

        return {vec.x / mag, vec.y / mag};
    }

    static float dot(const vec2f& a, const vec2f& b) {
        return a.x * b.x + a.y * b.y;
    }
    
    float x;
    float y;
};

constexpr vec2f k_up { 0.f, 1.f };
constexpr vec2f k_down { 0.f, -1.f };
constexpr vec2f k_right { 1.f, 0.f };
constexpr vec2f k_left { -1.f, 0.f };
constexpr vec2f k_zero { 0.f, 0.f };
inline string toString(const vec2f& v) {
    return { "(" + to_string(v.x) + ", " + to_string(v.y) + ")" };
}

// Approximation based on the pixel size on screen
constexpr int k_roverWidth = 250; 

inline float sign(float v) { return v >= 0 ? 1 : -1; }

class Landscape
{
public:
    int GetTargetLandingX() const { return m_closestLandingLocation.x; }
    vec2f GetTargetLandingLocation() const { return m_closestLandingLocation; };
    vec2f GetLandingZoneXInterval() const { return {(float)m_landingZoneStartX, (float)m_landingZoneEndX}; };

    void Initialize() {
        ReadLandcapeKeyPoints();
    }

    void CalculateClosestLandingX(int roverStartX) {
        const int roverSizeAdjustedStart = m_landingZoneStartX + k_roverWidth;
        const int roverSizeAdjustedEnd = m_landingZoneEndX - k_roverWidth;
        const int closestLandingX { roverStartX < roverSizeAdjustedStart ? roverSizeAdjustedStart : roverSizeAdjustedEnd };

        m_closestLandingLocation = vec2f{(float)closestLandingX, (float)m_landingZoneY};
    }

    bool IsPositionAboveGround(const vec2f& pos, bool bShowDebug) const {
        for(int i{1}; i < m_landscapeKeyPoints.size(); ++i)
        {
            const vec2f& point { m_landscapeKeyPoints[i] };
            if(point.x >= pos.x) {
                const vec2f& prevPoint { m_landscapeKeyPoints[i - 1] };

                const float ratioFromStart { (pos.x - prevPoint.x) / (point.x - prevPoint.x) };
                const float groundY { prevPoint.y + (point.y - prevPoint.y) * ratioFromStart };
                if(bShowDebug) {
                    cerr << "[IsPositionAboveGround] TestPosition: " << toString(pos) << endl;
                    cerr << "land point a: " << toString(prevPoint) << "land point b: " << toString(point) << endl;
                }
                return pos.y > groundY;
            }
        }
        return false;
    }

    bool IsPositionWithinLandingZone(const vec2f& pos) const {
        return pos.x >= m_landingZoneStartX && pos.x <= m_landingZoneEndX && pos.y >= m_landingZoneY;
    }

    bool IsXWithinLandingZone(float x) const {
        return x >= m_landingZoneStartX && x <= m_landingZoneEndX;
    }

private:
    void ReadLandcapeKeyPoints()
    {
        int n; // the number of points used to draw the surface of Mars.
        cin >> n; cin.ignore();

        int prevY = {INT_MAX};
        for (int i = 0; i < n; i++) {
            int land_x; // X coordinate of a surface point. (0 to 6999)
            int land_y; // Y coordinate of a surface point. By linking all the points together in a sequential fashion, you form the surface of Mars.
            cin >> land_x >> land_y; cin.ignore();
        
            m_landscapeKeyPoints.push_back({(float)land_x, (float)land_y});
        
            if(prevY == land_y) {
                m_landingZoneEndX = land_x;
                m_landingZoneY = land_y;
            }
            else if (m_landingZoneEndX == -1) {
                m_landingZoneStartX = land_x;
            }

            prevY = land_y;
        }
    }

    vector<vec2f> m_landscapeKeyPoints;

    int m_landingZoneStartX {-1};
    int m_landingZoneEndX {-1};
    int m_landingZoneY {-1};

    vec2f m_closestLandingLocation;
};

class Rover
{
public:
    Rover() {
        UpdateRoverData();
    }

    vec2f GetPosition() const {return m_position;}
    vec2f GetSpeed() const { return m_speed; }
    float GetTiltAngle() const { return m_tiltAngle; }
    float GetThrust() const { return m_thrust; }
    int GetFuelRemaining() const { return m_fuelRemaining; }

    void UpdateRoverData() {
        cin >> m_position.x >> m_position.y >> m_speed.x >> m_speed.y >> m_fuelRemaining >> m_tiltAngle >> m_thrust; cin.ignore();
    }

private:
    vec2f m_position;
    vec2f m_speed;
    int m_fuelRemaining {-1};
    float m_tiltAngle {};
    float m_thrust {-1};
};

constexpr vec2f k_gravity { 0.f, -3.711f };

void PlotRoverTrajectory(const Rover& rover, const Landscape& landscape, float& outDesiredDir) {
    // speed(t) = initial_speed + acceleration * t
    // position(t) = initial_position + initial_speed * t + 1/2 * acceleration * t^2
    // 
    // calculate acceleration vector (combining power/orientation and gravity)
    // position += speed + 1/2 * acceleration
    // speed += acceleration

    outDesiredDir = 0.f;

    vec2f roverPos { rover.GetPosition() };
    vec2f roverSpeed { rover.GetSpeed() };
    int roverFuel { rover.GetFuelRemaining() };

    const vec2f roverFw { vec2f::rotate(k_up, rover.GetTiltAngle()) };
    vec2f roverThrust { roverFw * rover.GetThrust() };
    if(isNearlyZero(roverSpeed.x)) {
        vec2f roverPos { rover.GetPosition() };
        if(roverPos.x < landscape.GetLandingZoneXInterval().x) {
            outDesiredDir = 1;
        }
        else if(roverPos.x > landscape.GetLandingZoneXInterval().y) {
            outDesiredDir = -1;
        }
        return;
    }
    
    vec2f roverAccel { roverThrust + k_gravity };
    

    float prevXDirection { sign(roverSpeed.x) };
    do {
        roverPos += roverSpeed + roverAccel * 0.5;
        roverSpeed += roverAccel;

        roverFuel -= rover.GetThrust();
        if(roverFuel <= 0) {
            roverAccel = k_gravity;
        }

        if(roverPos.x < 0 || roverPos.x > 7000)
            break;
        
        if(sign(roverSpeed.x) != prevXDirection)
            break;
        
        prevXDirection = sign(roverSpeed.x);
    } while (landscape.IsPositionAboveGround(roverPos, false));

    if(roverPos.x < landscape.GetLandingZoneXInterval().x)
    {
        outDesiredDir = 1;
    }
    else if (roverPos.x > landscape.GetLandingZoneXInterval().y)
    {
        outDesiredDir = -1;
    }
    else if (landscape.IsXWithinLandingZone(roverPos.x) && abs(roverSpeed.x) > 20)
    {
        outDesiredDir = -sign(roverSpeed.x);
    }
}

int main()
{
    Landscape landscape;
    landscape.Initialize();

    Rover rover;
    
    landscape.CalculateClosestLandingX(rover.GetPosition().x);

    const vec2f landingZonePos {landscape.GetTargetLandingLocation()};

    const int kP {5}, kI {0}, kD {3};
    float prevError {0};
    float prevDir{0};

    int R,P;
    // game loop
    while (1) {
        // First - plot the rover movement under the current condition to get an estimate of where it will end up
        float desiredDir;
        PlotRoverTrajectory(rover, landscape, desiredDir);
        if(desiredDir == 0.f) {
            desiredDir = prevDir;
        }

        // Second - apply PD controller to calculate the tilt angle input for the current tick
        // PID with the Error being the tilt error
        const float currentRoverTilt { rover.GetTiltAngle() };
        const vec2f currentRoverPos { rover.GetPosition() };
        const vec2f travelToLandingPos { landingZonePos - currentRoverPos };
        const vec2f travelDir { vec2f::getNormal(travelToLandingPos) };
        const float travelHorizonDot {vec2f::dot(k_right, travelDir)};
        const float desiredSlopeAngleRad {acos(travelHorizonDot)};
        const float desiredSlopeAngleDeg {rad2deg(desiredSlopeAngleRad)};
        const float desiredTiltAngleDeg {-(90.f - desiredSlopeAngleDeg)};

        const float error { desiredTiltAngleDeg - currentRoverTilt };

        const float PID_p { kP * error };
        const float PID_d { kD * (error - prevError) };

        const float PID_total { PID_p + PID_d };

        const float PID_dir { -desiredDir };
        const float PID_mag { abs(PID_total) };

        

        // Third - map the output of the PD controller to the rover input
        // This step requires tuning to the specific circumstance
        // Mapping the R input
        if(landscape.IsPositionWithinLandingZone(currentRoverPos) && abs(rover.GetSpeed().x) < 20) {
            R = 0;
        }
        else {
            R = PID_dir * min(PID_mag, 30.f);
        }

        // Mapping the P input
        if (R != 0) {
            P = abs(R) / 30.f * 4.f;
            if(rover.GetPosition().y >= 2925) {
                P /= 2;
            }
            else if (rover.GetSpeed().y <= -40) {
                P = 4;
            }
        }
        else {
            P = 0;
            if(rover.GetSpeed().y <= -30) {
                const float ratio {(abs(rover.GetSpeed().y) - 30.f) * 0.1f};
                P = ratio * 4.f;
            }
        }
         
        cout << R << " " << P << endl;

        prevError = error;
        rover.UpdateRoverData();
        prevDir = desiredDir;
    }
}
