import cls from "@/utils/cls"

import { Message } from "@/components/UI"
import { IMessage } from "@/types"

import styles from "./Chat.module.css"

interface ChatProps {
    className?: string
    data: IMessage[] | null
}

export const Chat = ({
    className = "",
    data
}: ChatProps) => (
    <>
        { data ? (
            <ul className={cls(styles.messages, className)}>
                { data.map((item: IMessage, index: number): JSX.Element => (
                    <Message key={index} variant={item.author === "me" ? "right" : "left"}>{item.text}</Message>
                )) }
            </ul>
        ) : (
                <div>It's empty</div>
            )
        }
    </>
)
