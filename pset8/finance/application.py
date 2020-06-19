import os
import time
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Create lists of dictionaries for user orders.
userOrders = [{}]
portfolio = {}
stocksOwned = []

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Begin by tranferring all relevent data from database on the users orders.
    userOrders.clear()
    portfolio.clear()
    stocksOwned.clear()
    rows = db.execute("SELECT orders.symbol, orders.name, orders.shares, orders.price, orders.timestamp FROM orders INNER JOIN users ON users.id = orders.user_id WHERE user_id = :id",
                        id = session["user_id"])

    # Populate userOrders dictionary with all of the users orders from the database.
    for i in range(len(rows)):
        infoDict = {"symbol": rows[i]["symbol"],
                    "name": rows[i]["name"],
                    "shares": rows[i]["shares"],
                    "price" : rows[i]["price"],
                    "timestamp": rows[i]["timestamp"]}
        userOrders.append(infoDict)


    # Retrieve the amount of cash the user has left.
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
    userCash = cash[0]["cash"]

    # Calculate the users equity.
    equityShares = db.execute("SELECT symbol, shares FROM userPortfolio WHERE user_id = :id", id = session["user_id"])
    stockEquity = 0
    for i in range(len(equityShares)):
        stockEquity = stockEquity + (equityShares[i]["shares"] * lookup(equityShares[i]["symbol"])["price"])

    userEquity = float(userCash) + float(stockEquity)

    # Get list of stocks the user owns.
    ownedStocks = db.execute("SELECT symbol FROM userPortfolio WHERE user_id = :id", id = session["user_id"])
    for row in ownedStocks:
        stocksOwned.append(row["symbol"])

    for stock in stocksOwned:
        stockInfo = db.execute("SELECT name, shares FROM userPortfolio WHERE symbol = :symbol AND user_id = :id",
                                symbol = stock, id = session["user_id"])
        portfolio[stock] = {"name": stockInfo[0]["name"],
                            "shares": stockInfo[0]["shares"],
                            "price": lookup(stock)["price"]}

    return render_template("portfolio.html", portfolio=portfolio, userCash=usd(userCash), userEquity=userEquity)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    stocksOwned.clear()
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure a symbol was submitted
        if not request.form.get("symbol") or lookup(request.form.get("symbol")) == None:
            return apology("must provide symbol/symbol does not exist", 403)

        # Ensure the amount of shares was submitted.
        elif not request.form.get("shares") or int(request.form.get('shares')) < 1:
            return apology("must provide amount of shares to purchase", 403)

        # Lookup the current price for 1 share of the symbol requested.
        sharePrice = float(lookup(request.form.get("symbol"))['price'])

        # Lookup how much cash the user currently has.
        rows = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])
        userCash = rows[0]["cash"]

        # If they cannot afford the purchase display it to the user.
        if sharePrice * int(request.form.get("shares")) > userCash:
            return apology("You cannot afford this purchase.")

        # If they can afford the purchase make the purchase for them.
        else:
            # Calculate cash remaining after purchase.
            cashRemaining = userCash - (sharePrice * int(request.form.get("shares")))

            # Update the users cash in the database.
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = cashRemaining, id = session["user_id"])

            # Figure out the exact date this purchase is occuring.
            ts = time.time()
            timeStamp = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')

            # Insert the information into the orders table in the database.
            db.execute("INSERT INTO orders (user_id, symbol, name, shares, price, timestamp) VALUES (:id, :symbol, :name, :shares, :price, :timestamp)",
                id = session["user_id"], symbol = request.form.get("symbol"), name = lookup(request.form.get("symbol"))["name"],
                shares = request.form.get("shares"), price = sharePrice, timestamp = timeStamp)

            # Get list of stocks the user owns.
            ownedStocks = db.execute("SELECT symbol FROM userPortfolio WHERE user_id = :id", id = session["user_id"])
            for row in ownedStocks:
                stocksOwned.append(row["symbol"])

            # Insert/Update the userPortfolio table in the database.
            if request.form.get("symbol") not in stocksOwned:
                db.execute("INSERT INTO userPortfolio (user_id, symbol, name, shares) VALUES (:id, :symbol, :name, :shares)",
                            id = session["user_id"], symbol = request.form.get("symbol"), name = lookup(request.form.get("symbol"))["name"],
                            shares = request.form.get("shares"))

            else:
                # Retrieve the amount of shares owned of the stock already.
                sharesOwned = db.execute("SELECT shares FROM userPortfolio WHERE symbol = :symbol", symbol = request.form.get("symbol"))
                updateCount = sharesOwned[0]["shares"] + int(request.form.get("shares"))
                db.execute("UPDATE userPortfolio SET shares = :shares WHERE symbol = :symbol", shares = updateCount, symbol = request.form.get("symbol"))

            # Confirm purchase.
            flash("Bought!")
            return redirect("/")


    # User reached route via GET (as by submitting a form via GET)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return render_template("history.html", userOrders=userOrders)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        if lookup(request.form.get("symbol")) != None:

            # Grab the stock information.
            name = lookup(request.form.get("symbol"))["name"]
            price = usd(lookup(request.form.get("symbol"))["price"])
            symbol = lookup(request.form.get("symbol"))["symbol"]

            # Return the stock information.
            return render_template("quoted.html", name=name, price=price, symbol=symbol)

    # User reached route via GET (as by submitting a form via GET)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure the password confirmation was submitted and matches the password.
        elif not request.form.get("confirmation") or request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) == 1:
            return apology("user already exists", 403)

        # Store login information in database.
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)",
            username = request.form.get("username"), password = generate_password_hash(request.form.get("password")))

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Registered!")
        return redirect("/")

    # User reached route via GET (as by submitting a form via GET)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    stocksOwned.clear()
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol of stock to sell", 403)

        # Ensure amount of shares was submitted
        elif not request.form.get("shares") or int(request.form.get('shares')) < 1 or int(request.form.get("shares")) > portfolio[request.form.get("symbol")]["shares"]:
            return apology("input a correct amount of shares", 403)

        # Sell the stock, remove the amount of shares from the userPortfolio table and from portfolio dictionary.
        # Update the cash to reflect the amount of money they received for selling the shares.
        moneyFromSharesSold = int(request.form.get("shares")) * lookup(request.form.get("symbol"))["price"]
        userCash = db.execute("SELECT cash FROM users where id = :id", id = session["user_id"])
        updateCash = userCash[0]["cash"] + moneyFromSharesSold
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = updateCash, id = session["user_id"])

        # Update portfolio dictionary and userPortfolio table on database to reflect the correct amount of shares after sale.
        # If amount of shares is 0 delete the entries.
        # Else update to the correct amount.
        sharesPreSaleDict = portfolio[request.form.get("symbol")]["shares"]
        if sharesPreSaleDict - int(request.form.get("shares")) == 0:
            del portfolio[request.form.get("symbol")]
        else:
            portfolio[request.form.get("symbol")]["shares"] = sharesPreSaleDict - int(request.form.get("shares"))

        sharesPreSaleDB = db.execute("SELECT shares FROM userPortfolio WHERE symbol = :symbol AND user_id = :id",
                                    symbol = request.form.get("symbol"), id = session["user_id"])
        if sharesPreSaleDB[0]["shares"] - int(request.form.get("shares")) == 0:
            db.execute("DELETE FROM userPortfolio WHERE symbol = :symbol AND user_id = :id",
                        symbol = request.form.get("symbol"), id = session["user_id"])
        else:
            db.execute("UPDATE userPortfolio SET shares = :shares WHERE symbol = :symbol AND user_id = :id",
                        shares = sharesPreSaleDB[0]["shares"] - int(request.form.get("shares")),
                        symbol = request.form.get("symbol"), id = session["user_id"])

        # Add the sell order to the orders database table.
        # Figure out the exact date this purchase is occuring.
        ts = time.time()
        timeStamp = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')

        # Lookup the current price for 1 share of the symbol requested.
        sharePrice = float(lookup(request.form.get("symbol"))['price'])

        # Insert the information into the orders table in the database.
        db.execute("INSERT INTO orders (user_id, symbol, name, shares, price, timestamp) VALUES (:id, :symbol, :name, :shares, :price, :timestamp)",
            id = session["user_id"], symbol = request.form.get("symbol"), name = lookup(request.form.get("symbol"))["name"],
            shares = "-" + request.form.get("shares"), price = sharePrice, timestamp = timeStamp)

        flash("Sold!")
        return redirect("/")

    else:
        # Get list of stocks the user owns.
        ownedStocks = db.execute("SELECT symbol FROM userPortfolio WHERE user_id = :id", id = session["user_id"])
        for row in ownedStocks:
            stocksOwned.append(row["symbol"])
        return render_template("sell.html", stocksOwned=stocksOwned)

@app.route("/fund", methods=["GET", "POST"])
@login_required
def fund():
    """Fund account with more cash"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("fund"):
            return apology("must provide with value to be funded", 403)

        # Retrieve amount of funds to be added and the current user cash.
        addedFunds = request.form.get("fund")
        userCash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])

        updatedCash = userCash[0]["cash"] + float(addedFunds)

        # Update the cash on the user database.
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash = updatedCash, id = session["user_id"])

        flash("Funded!")
        return redirect("/")

    else:
        return render_template("fund.html")

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
