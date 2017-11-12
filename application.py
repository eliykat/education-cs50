import os
import re
from flask import Flask, jsonify, render_template, request, url_for
from flask_jsglue import JSGlue

from cs50 import SQL
from helpers import lookup

# configure application
app = Flask(__name__)
JSGlue(app)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///mashup.db")

@app.route("/")
def index():
    """Render map."""
    if not os.environ.get("API_KEY"):
        raise RuntimeError("API_KEY not set")
    return render_template("index.html", key=os.environ.get("API_KEY"))

@app.route("/articles")
def articles():
    """Look up articles for geo."""

    articles = lookup(request.args.get('geo'))

    #The first result is always a warning that the URL feed is deprecated, so tidy this up
    #omitted for submission because this is probably unexpected behaviour
    #articles.pop(0);

    return jsonify(articles)

@app.route("/search")
def search():
    """Search for places that match query."""
    #TODO: this requires commas in the query to delineate place names.  Make it work with spaces as well

    #Get the query
    q = request.args.get("q")

    #Swap all plus signs for spaces
    q = q.replace("+", " ")
    #Delete all commas as they can interfere with SQL searches
    q = q.replace(",", "")

    #Split q into individual words
    q = q.split(" ")

    #The CS50 library doesn't support referencing out to an array, so we're being lazy and just copying them to regular variables
    #This supports a maximum of 3 words.  The first two are assumed to be place names and the last is assumed to be a state name or code.
    q0 = q[0] + '%'
    if len(q) > 1:
        q1 = q0 + q[1] + '%'
    else:
        q1 = q0

    last = '%' + q[len(q)-1] + '%'

    #This query returns the following, in this order:
    # matching postcode
    # matching place name (trying both one and two word names) and state (name or code)
    # matching place name only

    places = db.execute("SELECT * FROM places WHERE \
        postal_code LIKE :q0 OR \
        admin_name1 LIKE :q0 OR \
        admin_name1 LIKE :q1 OR \
        admin_code1 LIKE :q0 OR \
        (place_name LIKE :q0 AND admin_name1 LIKE :last) OR \
        (place_name LIKE :q0 AND admin_code1 LIKE :last) OR \
        (place_name LIKE :q1 AND admin_name1 LIKE :last) OR \
        (place_name LIKE :q1 AND admin_code1 LIKE :last) OR \
        place_name LIKE :q0 OR \
        place_name LIKE :q1 \
        ORDER BY CASE \
            WHEN postal_code LIKE :q0 THEN 0 \
            WHEN admin_name1 LIKE :q1 THEN 0 \
            WHEN admin_code1 LIKE :q0 THEN 1 \
            WHEN admin_name1 LIKE :q0 THEN 1 \
            WHEN (place_name LIKE :q1 AND admin_name1 LIKE :last) THEN 2 \
            WHEN (place_name LIKE :q1 AND admin_code1 LIKE :last) THEN 3 \
            WHEN (place_name LIKE :q0 AND admin_name1 LIKE :last) THEN 4 \
            WHEN (place_name LIKE :q0 AND admin_code1 LIKE :last) THEN 5 \
            WHEN place_name LIKE :q1 THEN 6 \
            WHEN place_name LIKE :q0 THEN 7 \
        ELSE 8 \
        END\
        ", q0=q0, q1=q1, last=last)

    return jsonify(places)

@app.route("/update")
def update():
    """Find up to 10 places within view."""

    # ensure parameters are present
    if not request.args.get("sw"):
        raise RuntimeError("missing sw")
    if not request.args.get("ne"):
        raise RuntimeError("missing ne")

    # ensure parameters are in lat,lng format
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("sw")):
        raise RuntimeError("invalid sw")
    if not re.search("^-?\d+(?:\.\d+)?,-?\d+(?:\.\d+)?$", request.args.get("ne")):
        raise RuntimeError("invalid ne")

    # explode southwest corner into two variables
    (sw_lat, sw_lng) = [float(s) for s in request.args.get("sw").split(",")]

    # explode northeast corner into two variables
    (ne_lat, ne_lng) = [float(s) for s in request.args.get("ne").split(",")]

    # find 10 cities within view, pseudorandomly chosen if more within view
    if (sw_lng <= ne_lng):

        # doesn't cross the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude AND longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    else:

        # crosses the antimeridian
        rows = db.execute("""SELECT * FROM places
            WHERE :sw_lat <= latitude AND latitude <= :ne_lat AND (:sw_lng <= longitude OR longitude <= :ne_lng)
            GROUP BY country_code, place_name, admin_code1
            ORDER BY RANDOM()
            LIMIT 10""",
            sw_lat=sw_lat, ne_lat=ne_lat, sw_lng=sw_lng, ne_lng=ne_lng)

    # output places as JSON
    return jsonify(rows)
