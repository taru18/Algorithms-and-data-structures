// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <unordered_map>
#include <map>
#include <memory>
#include <list>

// Type for beacon IDs
using BeaconID = std::string;

// Return value for cases where required beacon was not found
BeaconID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for color (RGB)
struct Color
{
    int r = NO_VALUE;
    int g = NO_VALUE;
    int b = NO_VALUE;
};

// Equality and non-equality comparisons for Colors
inline bool operator==(Color c1, Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b; }
inline bool operator!=(Color c1, Color c2) { return !(c1==c2); }

// Return value for cases where color was not found
Color const NO_COLOR = {NO_VALUE, NO_VALUE, NO_VALUE};

// Type for light transmission cost (used only in the second assignment)
using Cost = int;

// Return value for cases where cost is unknown
Cost const NO_COST = NO_VALUE;

struct Beacon {

    BeaconID id = NO_ID;
    Coord coord = NO_COORD;
    std::string name = NO_NAME;
    Color color = NO_COLOR;
    std::shared_ptr<Beacon> sending = nullptr;
    std::vector<std::shared_ptr<Beacon>> receiving = {};
    int brightness = 0;

};

struct Xpoint {

    Coord coord = NO_COORD;
    std::map<Coord, Cost> fibres = {};
    bool visited = false;
};

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: ei rekursiivinen eikä looppi
    int beacon_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: jokainen elementti postetaan yksitellen, kaikki mapit yhtä isoja
    void clear_beacons();

    // Estimate of performance: O(n)
    // Short rationale for estimate: mapin läpi käynti O(n), vectorin loppuun lisääminen O(1)
    std::vector<BeaconID> all_beacons();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: lisäykset multimappeihin O(logn), muut O(1)
    bool add_beacon(BeaconID id, std::string const& name, Coord xy, Color color);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Etsitään alkiota (O(1)), jonka nimi palautettaan
    std::string get_name(BeaconID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Etsitään alkiota (O(1)), jonka koordinaatit palautetaan
    Coord get_coordinates(BeaconID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: Etsitään alkiota (O(1)), jonka väri palautetaan
    Color get_color(BeaconID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n)
    // Short rationale for estimate: mapin läpikäynti O(n) ja vectorin loppuun lisäys O(1)
    std::vector<BeaconID> beacons_alphabetically();

    // Estimate of performance: O(n)
    // Short rationale for estimate: mapin läpikäynti O(n) ja vectorin loppuun lisäys O(1)
    std::vector<BeaconID> beacons_brightness_increasing();

    // Estimate of performance: O(1)
    // Short rationale for estimate: ei riipu majakoiden määrästä, empty() ja begin() O(1)
    BeaconID min_brightness();

    // Estimate of performance: O(1)
    // Short rationale for estimate: ei riipu majakoiden määrästä, empty() ja rbegin() O(1)
    BeaconID max_brightness();

    // Estimate of performance: O(m+nlogn)
    // Short rationale for estimate: mapin läpikäynti m ja paluuvectorin järjestäminen nlogn
    std::vector<BeaconID> find_beacons(std::string const& name);

    // Estimate of performance: O(logn + m)
    // Short rationale for estimate: equal_range logn, for looppi m, poisto n, lisäys multimappiin logn, loput O(1)
    bool change_beacon_name(BeaconID id, std::string const& newname);

    // Estimate of performance: O(logn + m)
    // Short rationale for estimate: equal_range logn, for looppi m, poisto n, lisäys multimappiin logn, loput O(1)
    bool change_beacon_color(BeaconID id, Color newcolor);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(1)
    // Short rationale for estimate: kaikki operaatiot O(1)
    bool add_lightbeam(BeaconID sourceid, BeaconID targetid);

    // Estimate of performance: O(m+nlogn)
    // Short rationale for estimate: for looppi m, lajittelu nlogn
    std::vector<BeaconID> get_lightsources(BeaconID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: vectorin järjestäminen O(n)
    std::vector<BeaconID> path_outbeam(BeaconID id);

    // Non-compulsory operations

    // Estimate of performance: tiivistettynä O(logn+n)
    // Short rationale for estimate: equal_range logn, for loopit m+n+o,
    // poistot 1 (unordered_map) ja logn (multimap), loput O(1)
    bool remove_beacon(BeaconID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<BeaconID> path_inbeam_longest(BeaconID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: kaikki operaatiot O(1)
    Color total_color(BeaconID id);

    // Phase 2 operations

    // Estimate of performance: O(nlogn)
    // Short rationale for estimate: mapin läpikäynti O(n), std::find O(logn),
    // vektoriin lisäys O(1)
    std::vector<Coord> all_xpoints();

    // Estimate of performance: O(logn)
    // Short rationale for estimate: alkioiden etsimiset O(logn), mappeihin lisäykset O(logn)
    bool add_fibre(Coord xpoint1, Coord xpoint2, Cost cost);

    // Estimate of performance: O(n)
    // Short rationale for estimate: mapin läpikäynti O(n), vektoriin lisääminen O(1)
    std::vector<std::pair<Coord, Cost>> get_fibres_from(Coord xpoint);

    // Estimate of performance: O(n)
    // Short rationale for estimate: mapin läpikäynti O(n) ja vektoriin lisäys O(1)
    std::vector<std::pair<Coord, Coord>> all_fibres();

    // Estimate of performance: O(2n)
    // Short rationale for estimate: erase:t O(n)
    bool remove_fibre(Coord xpoint1, Coord xpoint2);

    // Estimate of performance: O(2n)
    // Short rationale for estimate: jokainen elementti poistetaan yksitellen molemmista mapeista
    void clear_fibres();

    // Estimate of performance: O(n^x + m)
    // Short rationale for estimate: hyödyntää bfsRoutea ja route_least_xpointsia
    std::vector<std::pair<Coord, Cost>> route_any(Coord fromxpoint, Coord toxpoint);

    // Non-compulsory operations

    // Estimate of performance: O(n^x + m)
    // Short rationale for estimate: hyödyntää bfsRoutea O(n^x) ja käy m verran läpi koordinaatteja
    std::vector<std::pair<Coord, Cost>> route_least_xpoints(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Coord, Cost>> route_fastest(Coord fromxpoint, Coord toxpoint);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<Coord> route_fibre_cycle(Coord startxpoint);

    // Estimate of performance:
    // Short rationale for estimate:
    Cost trim_fibre_network();

private:

    // Funktio majakan kirkkauden päivittämiseen manuaalisesti
    // Estimate of performance: O(1)
    // Short rationale for estimate: suoritetaan O(1) operaatiot vain yhdelle alkiolle
    void update_brightness(BeaconID id);

    // Rekursiivinen funktio valonsäteiden selvittämiseen
    // Estimate of performance: O(n)
    // Short rationale for estimate: rekursiivinen funktio, jossa alkioita lisätään vectorin loppuun O(1)
    std::vector<BeaconID> path_recursive(BeaconID);

    // Rekursiivinen funktio majakoiden värien laskemiseen
    // Estimate of performance: O(n)
    // Short rationale for estimate: rekursiivinen funktio, jossa mahdollisen vectorin läpikäynti O(n) ja laskutoimitukset O(1)
    Color color_recursive(BeaconID);

    // Unordered_map for saving all beacons' data
    std::unordered_map<BeaconID, Beacon> allBeacons;

    // Map for arranging beacons by name
    std::multimap<std::string, BeaconID> beaconNames;

    // Map for arranging beacons by brightness
    std::multimap<int, BeaconID> beaconBrightnesses;

    // Estimate of performance: O(n^x)
    // Short rationale for estimate: käydään n alkioiden kohdalla m alkioisia listoja läpi
    std::vector<Coord> bfsRoute(Coord from, Coord to);

    // Estimate of performance: O(1)
    // Short rationale for estimate: kaikki operaatiot O(1)
    std::pair<Coord, Coord> swapCoords(std::pair<Coord, Coord> point);

    // Map for saving fibre coordpairs and costs
    std::map<std::pair<Coord, Coord>, Cost> fibreCoords;

    // Map for saving Xpoints
    std::map<Coord, Xpoint> allFibres;

};

#endif // DATASTRUCTURES_HH
