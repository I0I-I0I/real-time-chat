import { DB_URL } from "@/consts"
import { IFetchData, IPostUser, IUser } from "@/types"

const URL = DB_URL

export default class AuthService {
    static async register(post_data: IPostUser): Promise<IFetchData<IUser> | null>  {
        const resp = await fetch(URL + "/register", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                login: post_data.login,
                username: post_data.username,
                password: post_data.password
            }])
        })
        if (resp.status !== 200 && resp.status !== 422) {
            return null
        }
        const data = await resp.json() as IFetchData<IUser>
        return data
    }

    static async loginWithHash(hash: string): Promise<IUser | null> {
        const resp = await fetch(URL + "/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                hash: hash
            }])
        })
        if (resp.status !== 200 && resp.status !== 401) {
            return null
        }
        const data = await resp.json() as IFetchData<IUser>
        return data.data[0]
    }

    static async loginWithPassword(login: string, password: string): Promise<IFetchData<IUser> | null> {
        const resp = await fetch(URL + "/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                login: login,
                password: password
            }])
        })
        if (resp.status !== 200 && resp.status !== 401) {
            return null
        }
        return await resp.json() as IFetchData<IUser>
    }

    static async logout(login: string, hash: string): Promise<number | null>  {
        const resp = await fetch(URL + "/logout", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify([{
                login: login,
                hash: hash
            }])
        })
        if (resp.status === 200) {
            return resp.status
        }
        return null
    }
}
