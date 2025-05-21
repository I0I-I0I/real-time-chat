import { DB_URL } from "@/consts"
import { IFetchData, IMessage, IMessagePost } from "@/types"

const URL = DB_URL + "/messages"

export default class MessageService {
    static async getById(id: number, chatId: number): Promise<IMessage | null> {
        const resp = await fetch(URL + "?chatId=" + chatId + "&id=" + id, {
            method: "GET",
        })
        const data = (await resp.json()) as IFetchData<IMessage>
        if (data.status !== 200) {
            return null
        }
        return data.data[0]
    }

    static async getAll(chatId: number): Promise<IMessage[] | null> {
        const resp = await fetch(URL + "?chatId=" + chatId, {
            method: "GET",
        })

        if (resp.status === 205) return []

        const data = (await resp.json()) as IFetchData<IMessage>
        if (data.status !== 200) {
            return null
        }
        return data.data
    }

    static async createOne(post_data: IMessagePost): Promise<IMessage | null> {
        const resp = await fetch(URL, {
            method: "POST",
            headers: {
                "Content-Type": "application/json",
            },
            body: JSON.stringify([
                {
                    chatId: "" + post_data.chatId,
                    authorId: "" + post_data.authorId,
                    body: post_data.body,
                },
            ]),
        })
        const data = (await resp.json()) as IFetchData<IMessage>
        if (data.status === 200) {
            return data.data[0]
        }
        return null
    }

    static async removeOne(id: number): Promise<number | null> {
        const resp = await fetch(URL + "?id=" + id, {
            method: "DELETE",
        })
        const data = (await resp.json()) as IFetchData<IMessage>
        if (data.status === 200) {
            return data.status
        }
        return null
    }
}
